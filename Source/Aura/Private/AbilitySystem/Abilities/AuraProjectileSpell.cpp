// Copyright Druid Mechanics

/*
 * AuraProjectileSpell.cpp
 * 투사체(Projectile) 기반 마법 어빌리티 구현
 *
 * 투사체를 발사하는 마법 스킬의 기본 클래스입니다.
 * 소켓 위치에서 투사체를 스폰하고 타겟 방향으로 발사합니다.
 */

#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

/** 어빌리티 활성화 - 블루프린트에서 구체적인 로직 구현 */
void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
	
}

/**
 * 투사체 스폰
 * 지정된 소켓 위치에서 타겟 방향으로 투사체를 생성합니다.
 *
 * @param ProjectileTargetLocation - 투사체가 향할 목표 위치
 * @param SocketTag - 발사 위치 소켓 태그 (Weapon, LeftHand 등)
 * @param bOverridePitch - 피치 각도 오버라이드 여부
 * @param PitchOverride - 오버라이드할 피치 각도
 */
void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	// 서버에서만 투사체 스폰 (클라이언트는 복제로 받음)
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
	Projectile->FinishSpawning(SpawnTransform);
}
