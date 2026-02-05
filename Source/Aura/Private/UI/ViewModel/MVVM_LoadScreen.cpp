// Copyright Druid Mechanics

/**
 * MVVM_LoadScreen.cpp
 *
 * 로드 화면 뷰모델 클래스 구현 파일
 *
 * MVVM 패턴에서 ViewModel 역할을 담당합니다.
 * 로드 화면의 비즈니스 로직과 데이터 바인딩을 관리합니다.
 *
 * 관리하는 데이터:
 * - 세이브 슬롯 목록 (3개: LoadSlot_0, LoadSlot_1, LoadSlot_2)
 * - 현재 선택된 슬롯
 * - 슬롯 개수
 *
 * 제공하는 기능:
 * - 새 게임 생성
 * - 기존 게임 로드
 * - 세이브 삭제
 * - 슬롯 선택
 */

#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

/**
 * 로드 슬롯 초기화
 * 3개의 세이브 슬롯 뷰모델을 생성하고 초기화합니다.
 */
void UMVVM_LoadScreen::InitializeLoadSlots()
{
	// 슬롯 0 생성
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName(FString("LoadSlot_0"));
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);

	// 슬롯 1 생성
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_1->SlotIndex = 1;
	LoadSlot_1->SetLoadSlotName(FString("LoadSlot_1"));

	// 슬롯 2 생성
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlots.Add(2, LoadSlot_2);
	LoadSlot_2->SetLoadSlotName(FString("LoadSlot_2"));
	LoadSlot_2->SlotIndex = 2;

	// 슬롯 개수 설정 (MVVM 바인딩)
	SetNumLoadSlots(LoadSlots.Num());
}

/**
 * 인덱스로 슬롯 뷰모델 가져오기
 * @param Index - 슬롯 인덱스 (0, 1, 2)
 * @return 해당 슬롯의 뷰모델
 */
UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 Index) const
{
	return LoadSlots.FindChecked(Index);
}

/**
 * 새 슬롯 버튼 눌림 (캐릭터 이름 입력 후)
 * @param Slot - 슬롯 인덱스
 * @param EnteredName - 입력된 플레이어 이름
 *
 * 새 게임 데이터를 생성하고 저장합니다.
 */
void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameMode))
	{
		// 멀티플레이어 모드에서는 사용 불가
		GEngine->AddOnScreenDebugMessage(1, 15.f, FColor::Magenta, FString("Please switch to Single Player"));
		return;
	}

	// 슬롯 데이터 설정
	LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnteredName);
	LoadSlots[Slot]->SetPlayerLevel(1);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	LoadSlots[Slot]->MapAssetName = AuraGameMode->DefaultMap.ToSoftObjectPath().GetAssetName();

	// 게임 모드에 저장 요청
	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);
	LoadSlots[Slot]->InitializeSlot();

	// 게임 인스턴스에 현재 슬롯 정보 저장
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	AuraGameInstance->LoadSlotName = LoadSlots[Slot]->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
}

/**
 * 새 게임 버튼 눌림
 * @param Slot - 슬롯 인덱스
 *
 * 해당 슬롯의 UI를 이름 입력 화면으로 전환합니다.
 */
void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	// 위젯 스위처 인덱스 1 = 이름 입력 화면
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

/**
 * 슬롯 선택 버튼 눌림
 * @param Slot - 선택된 슬롯 인덱스
 *
 * 슬롯을 선택하고 다른 슬롯들의 선택 버튼을 활성화합니다.
 */
void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	// 슬롯 선택 이벤트 브로드캐스트 (Play/Delete 버튼 활성화)
	SlotSelected.Broadcast();

	// 선택된 슬롯은 버튼 비활성화, 나머지는 활성화
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
	SelectedSlot = LoadSlots[Slot];
}

/**
 * 삭제 버튼 눌림
 * 현재 선택된 슬롯의 세이브 데이터를 삭제합니다.
 */
void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		// 세이브 파일 삭제
		AAuraGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->SlotIndex);
		// 슬롯 상태 초기화
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->EnableSelectSlotButton.Broadcast(true);
	}
}

/**
 * 플레이 버튼 눌림
 * 선택된 슬롯의 게임을 시작합니다.
 */
void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());

	// 게임 인스턴스에 슬롯 정보 저장
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuraGameInstance->LoadSlotName = SelectedSlot->GetLoadSlotName();
	AuraGameInstance->LoadSlotIndex = SelectedSlot->SlotIndex;

	if (IsValid(SelectedSlot))
	{
		// 저장된 맵으로 이동
		AuraGameMode->TravelToMap(SelectedSlot);
	}
}

/**
 * 저장 데이터 로드
 * 모든 슬롯의 세이브 데이터를 로드하여 UI에 표시합니다.
 */
void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!IsValid(AuraGameMode)) return;

	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		// 각 슬롯의 세이브 데이터 로드
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);

		// UI에 데이터 설정
		const FString PlayerName = SaveObject->PlayerName;
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;

		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->InitializeSlot();

		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerLevel(SaveObject->PlayerLevel);
	}
}

/**
 * 슬롯 개수 설정 (MVVM 프로퍼티)
 * @param InNumLoadSlots - 슬롯 개수
 */
void UMVVM_LoadScreen::SetNumLoadSlots(int32 InNumLoadSlots)
{
	UE_MVVM_SET_PROPERTY_VALUE(NumLoadSlots, InNumLoadSlots);
}
