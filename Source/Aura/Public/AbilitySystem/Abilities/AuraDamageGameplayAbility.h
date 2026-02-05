// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraDamageGameplayAbility.generated.h"

/**
 * UAuraDamageGameplayAbility
 * 데미지를 주는 어빌리티의 기본 클래스
 * 모든 공격 스킬이 상속받아 사용합니다.
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:

	// 대상 액터에게 데미지를 가합니다
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	// 데미지 이펙트 파라미터를 클래스 기본값으로부터 생성
	// 넉백, 사망 임펄스, 범위 데미지 등 다양한 옵션을 설정할 수 있습니다
	UFUNCTION(BlueprintPure)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f) const;

	// 현재 레벨에서의 데미지 값을 반환
	UFUNCTION(BlueprintPure)
	float GetDamageAtLevel() const;
protected:

	// 데미지 게임플레이 이펙트 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 데미지 타입 (화염, 번개, 비전, 물리 등)
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	// 레벨에 따라 스케일되는 데미지 값
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	// 디버프 적용 확률 (%)
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;

	// 디버프로 입히는 초당 데미지
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;

	// 디버프 데미지 적용 빈도 (초)
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;

	// 디버프 지속 시간 (초)
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;

	// 사망 시 가해지는 임펄스 크기
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 1000.f;

	// 넉백 힘의 크기
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackForceMagnitude = 1000.f;

	// 넉백 발생 확률 (%)
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float KnockbackChance = 0.f;

	// 범위 데미지 여부
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsRadialDamage = false;

	// 범위 데미지 내부 반경 (최대 데미지)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageInnerRadius = 0.f;

	// 범위 데미지 외부 반경 (최소 데미지)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float RadialDamageOuterRadius = 0.f;

	// 태그가 지정된 몽타주 배열에서 랜덤한 하나를 선택
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
