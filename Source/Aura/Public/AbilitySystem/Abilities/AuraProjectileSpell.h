// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
class UGameplayEffect;
struct FGameplayTag;

/**
 * UAuraProjectileSpell
 * 투사체를 발사하는 스킬의 기본 클래스
 * 파이어볼트, 아케인 샤드 등의 투사체 스킬에 사용됩니다.
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()


protected:

	// 어빌리티 활성화 시 호출
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 투사체 생성 및 발사
	// ProjectileTargetLocation: 투사체 목표 위치
	// SocketTag: 발사 위치 소켓 태그
	// bOverridePitch: 피치 값 오버라이드 여부
	// PitchOverride: 오버라이드할 피치 값
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);

	// 생성할 투사체 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	// 한 번에 발사할 투사체 개수
	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
};
