// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

/**
 * FLootItem
 * 루트 아이템 구조체
 * 적 처치 시 드랍될 수 있는 아이템의 정보를 담습니다.
 */
USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()

	// 루트 클래스 (생성할 아이템 액터)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning")
	TSubclassOf<AActor> LootClass;

	// 생성 확률 (0.0 ~ 1.0)
	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	float ChanceToSpawn = 0.f;

	// 최대 생성 개수
	UPROPERTY(EditAnywhere, Category = "LootTiers|Spawning")
	int32 MaxNumberToSpawn = 0.f;

	// 루트 레벨 오버라이드 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTiers|Spawning")
	bool bLootLevelOverride = true;
};

/**
 * ULootTiers
 * 루트 티어 데이터 에셋
 * 적의 등급별 드랍 아이템 목록을 정의합니다.
 * 각 적 클래스는 고유한 루트 티어를 가질 수 있습니다.
 */
UCLASS()
class AURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()
public:

	// 루트 아이템 목록 가져오기
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();

	// 루트 아이템 배열
	UPROPERTY(EditDefaultsOnly, Category = "LootTiers|Spawning")
	TArray<FLootItem> LootItems;
};
