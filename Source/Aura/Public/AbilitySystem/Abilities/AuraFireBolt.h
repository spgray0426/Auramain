// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * UAuraFireBolt
 * 파이어볼트 스킬 - 유도 화염 투사체를 발사합니다.
 * 레벨에 따라 발사되는 투사체 개수가 증가하며, 적을 추적합니다.
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
public:
	// 현재 레벨의 스킬 설명 반환
	virtual FString GetDescription(int32 Level) override;

	// 다음 레벨의 스킬 설명 반환
	virtual FString GetNextLevelDescription(int32 Level) override;

	// 여러 개의 투사체를 생성하고 발사
	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:

	// 투사체 발사 분산 각도 (도 단위)
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectileSpread = 90.f;

	// 최대 발사 가능한 투사체 개수
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	int32 MaxNumProjectiles = 5;

	// 유도 가속도 최소값
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin = 1600.f;

	// 유도 가속도 최대값
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax = 3200.f;

	// 유도 투사체 발사 여부
	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bLaunchHomingProjectiles = true;
};
