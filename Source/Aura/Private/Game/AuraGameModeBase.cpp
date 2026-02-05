// Copyright Druid Mechanics

/**
 * AuraGameModeBase.cpp
 *
 * 게임 모드 베이스 클래스 구현 파일
 *
 * 게임의 규칙과 흐름을 관리하는 핵심 클래스입니다.
 * - 세이브/로드 시스템 관리
 * - 월드 상태 저장 및 복원
 * - 맵 이동(트래블) 처리
 * - 플레이어 시작 위치 선택
 * - 플레이어 사망 시 맵 재시작
 *
 * 게임 모드는 서버에서만 존재하며, 게임의 권위적인(Authoritative) 로직을 담당합니다.
 */

#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Aura/AuraLogChannels.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "GameFramework/Character.h"

/**
 * 슬롯 데이터 저장
 * @param LoadSlot - 저장할 슬롯의 뷰모델
 * @param SlotIndex - 슬롯 인덱스
 *
 * 기존 세이브가 있으면 삭제 후 새로 저장합니다.
 * 플레이어 이름, 맵 정보, 시작 위치 등을 저장합니다.
 */
void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	// 기존 세이브 파일이 있으면 삭제
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}

	// 새 세이브 게임 오브젝트 생성
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);

	// 세이브 데이터 설정
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->SaveSlotStatus = Taken;  // 슬롯 사용 중 상태로 설정
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;

	// 파일로 저장
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->GetLoadSlotName(), SlotIndex);
}

/**
 * 슬롯 데이터 불러오기
 * @param SlotName - 슬롯 이름
 * @param SlotIndex - 슬롯 인덱스
 * @return 세이브 게임 오브젝트 (없으면 새로 생성)
 */
ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		// 기존 세이브 파일 로드
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		// 세이브 파일이 없으면 새로 생성
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}

	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

/**
 * 슬롯 삭제
 * @param SlotName - 삭제할 슬롯 이름
 * @param SlotIndex - 슬롯 인덱스
 */
void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

/**
 * 현재 게임 진행 데이터 불러오기
 * 게임 인스턴스에 저장된 현재 슬롯 정보를 사용하여 세이브 데이터를 반환합니다.
 */
ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

/**
 * 게임 진행 데이터 저장
 * @param SaveObject - 저장할 세이브 게임 오브젝트
 *
 * 체크포인트 도달, 맵 이동 시 플레이어 진행 상황을 저장합니다.
 */
void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	// 플레이어 시작 위치 태그 업데이트
	AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

/**
 * 월드 상태 저장
 * @param World - 저장할 월드
 * @param DestinationMapAssetName - 이동 목적지 맵 이름 (맵 이동 시)
 *
 * 현재 맵의 모든 세이브 가능한 액터들의 상태를 직렬화하여 저장합니다.
 * ISaveInterface를 구현한 액터들만 저장됩니다.
 */
void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	// 맵 이름에서 스트리밍 레벨 접두사 제거
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		// 목적지 맵이 지정된 경우 맵 정보 업데이트
		if (DestinationMapAssetName != FString(""))
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}

		// 해당 맵의 저장 데이터가 없으면 새로 추가
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}

		// 기존 액터 데이터 초기화 후 새로 저장
		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();

		// 월드의 모든 액터를 순회하며 저장 가능한 액터 처리
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			// 유효하지 않거나 ISaveInterface를 구현하지 않은 액터는 건너뜀
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			// 액터의 SaveGame 플래그가 설정된 속성들을 바이너리로 직렬화
			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);

			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		// 저장된 맵 배열에서 해당 맵 데이터 교체
		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}

		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	}
}

/**
 * 월드 상태 불러오기
 * @param World - 상태를 복원할 월드
 *
 * 저장된 데이터를 사용하여 액터들의 상태를 복원합니다.
 */
void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{
		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load slot"));
			return;
		}

		// 월드의 모든 액터를 순회하며 저장된 상태 복원
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			// 저장된 액터 데이터 중 일치하는 것을 찾아 복원
			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					// Transform 복원 여부 확인 (체크포인트 등은 위치 복원 불필요)
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					// 바이너리 데이터를 역직렬화하여 속성 복원
					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);

					// 액터에게 로드 완료 알림 (추가 초기화 작업용)
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

/**
 * 맵으로 이동
 * @param Slot - 이동할 맵 정보가 담긴 슬롯 뷰모델
 */
void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->GetLoadSlotName();
	const int32 SlotIndex = Slot->SlotIndex;

	// 소프트 오브젝트 포인터를 사용하여 레벨 열기
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

/**
 * 맵 에셋 이름으로부터 맵 이름 조회
 * @param MapAssetName - 맵 에셋 이름
 * @return 사용자 친화적인 맵 이름
 */
FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

/**
 * 플레이어 시작 위치 선택 (오버라이드)
 * @param Player - 플레이어 컨트롤러
 * @return 선택된 PlayerStart 액터
 *
 * 게임 인스턴스의 PlayerStartTag와 일치하는 시작 위치를 찾습니다.
 * 체크포인트나 맵 입구에서 저장된 위치로 플레이어를 스폰합니다.
 */
AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);

	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];  // 기본값: 첫 번째 시작 위치

		// PlayerStartTag가 일치하는 시작 위치 찾기
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

/**
 * 플레이어 사망 처리
 * @param DeadCharacter - 사망한 캐릭터
 *
 * 플레이어 사망 시 마지막 저장된 맵으로 재시작합니다.
 */
void AAuraGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	// 저장된 맵을 다시 열어 플레이어 리스폰
	UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

/**
 * BeginPlay
 * 게임 시작 시 기본 맵을 맵 목록에 추가합니다.
 */
void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
