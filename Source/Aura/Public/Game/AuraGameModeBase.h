// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULootTiers;
class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;

/**
 * AAuraGameModeBase
 * Aura 게임의 게임 모드 클래스
 * 게임 규칙, 저장/로드, 맵 전환 등을 관리합니다.
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	// 캐릭터 클래스 정보 데이터 에셋
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	// 어빌리티 정보 데이터 에셋
	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	// 전리품 티어 데이터 에셋
	UPROPERTY(EditDefaultsOnly, Category = "Loot Tiers")
	TObjectPtr<ULootTiers> LootTiers;

	// 슬롯 데이터 저장
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

	// 저장 슬롯 데이터 가져오기
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	// 슬롯 삭제
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	// 인게임 저장 데이터 가져오기
	ULoadScreenSaveGame* RetrieveInGameSaveData();

	// 인게임 진행 상황 저장
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	// 월드 상태 저장
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;

	// 월드 상태 로드
	void LoadWorldState(UWorld* World) const;

	// 맵으로 이동
	void TravelToMap(UMVVM_LoadSlot* Slot);

	// 로드 스크린 세이브 게임 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	// 기본 맵 이름
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	// 기본 맵
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	// 기본 플레이어 시작 태그
	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayerStartTag;

	// 맵 이름과 맵 에셋의 매핑
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	// 맵 에셋 이름에서 맵 이름 추출
	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	// 플레이어 시작 위치 선택
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	// 플레이어 사망 처리
	void PlayerDied(ACharacter* DeadCharacter);
protected:
	virtual void BeginPlay() override;
	
};
