// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * UAuraSummonAbility
 * 소환 어빌리티 - 미니언(하수인)을 소환합니다.
 * 플레이어 주변에 여러 개의 미니언을 생성하여 전투를 보조합니다.
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	// 미니언을 소환할 위치 목록을 계산
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	// 랜덤한 미니언 클래스를 선택하여 반환
	UFUNCTION(BlueprintPure, Category="Summoning")
	TSubclassOf<APawn> GetRandomMinionClass();

	// 소환할 미니언 개수
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	// 소환 가능한 미니언 클래스 배열
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	// 최소 소환 거리
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	// 최대 소환 거리
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 250.f;

	// 소환 위치 분산 각도 (도 단위)
	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;


	
};
