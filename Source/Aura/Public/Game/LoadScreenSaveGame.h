// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

/**
 * ESaveSlotStatus
 * 저장 슬롯 상태
 */
UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,      // 비어있음
	EnterName,   // 이름 입력 중
	Taken        // 사용 중
};

/**
 * FSavedActor
 * 저장된 액터 정보
 */
USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

	// 액터 이름
	UPROPERTY()
	FName ActorName = FName();

	// 트랜스폼 (위치, 회전, 스케일)
	UPROPERTY()
	FTransform Transform = FTransform();

	// SaveGame 지정자가 있는 변수들의 직렬화된 데이터
	UPROPERTY()
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

/**
 * FSavedMap
 * 저장된 맵 정보
 */
USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

	// 맵 에셋 이름
	UPROPERTY()
	FString MapAssetName = FString();

	// 맵의 저장된 액터들
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
};

/**
 * FSavedAbility
 * 저장된 어빌리티 정보
 */
USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	// 어빌리티 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;

	// 어빌리티 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityTag = FGameplayTag();

	// 어빌리티 상태 (잠금/해제/장착)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityStatus = FGameplayTag();

	// 어빌리티 슬롯
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilitySlot = FGameplayTag();

	// 어빌리티 타입
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag AbilityType = FGameplayTag();

	// 어빌리티 레벨
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AbilityLevel = 1;
};

inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

/**
 * ULoadScreenSaveGame
 * 저장 데이터 클래스
 * 플레이어의 진행 상황, 능력치, 어빌리티, 월드 상태 등을 저장합니다.
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	// 슬롯 이름
	UPROPERTY()
	FString SlotName = FString();

	// 슬롯 인덱스
	UPROPERTY()
	int32 SlotIndex = 0;

	// 플레이어 이름
	UPROPERTY()
	FString PlayerName = FString("Default Name");

	// 맵 이름
	UPROPERTY()
	FString MapName = FString("Default Map Name");

	// 맵 에셋 이름
	UPROPERTY()
	FString MapAssetName = FString("Default Map Asset Name");

	// 플레이어 시작 위치 태그
	UPROPERTY()
	FName PlayerStartTag;

	// 저장 슬롯 상태
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	// 처음 로드하는지 여부
	UPROPERTY()
	bool bFirstTimeLoadIn = true;

	/* Player - 플레이어 정보 */

	// 플레이어 레벨
	UPROPERTY()
	int32 PlayerLevel = 1;

	// 경험치
	UPROPERTY()
	int32 XP = 0;

	// 주문 포인트
	UPROPERTY()
	int32 SpellPoints = 0;

	// 능력치 포인트
	UPROPERTY()
	int32 AttributePoints = 0;

	// 힘
	UPROPERTY()
	float Strength = 0;

	// 지능
	UPROPERTY()
	float Intelligence = 0;

	// 회복력
	UPROPERTY()
	float Resilience = 0;

	// 활력
	UPROPERTY()
	float Vigor = 0;

	/* Abilities - 어빌리티 정보 */

	// 저장된 어빌리티 목록
	UPROPERTY()
	TArray<FSavedAbility> SavedAbilities;

	// 저장된 맵 목록
	UPROPERTY()
	TArray<FSavedMap> SavedMaps;

	// 맵 이름으로 저장된 맵 가져오기
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);

	// 맵이 있는지 확인
	bool HasMap(const FString& InMapName);
};
