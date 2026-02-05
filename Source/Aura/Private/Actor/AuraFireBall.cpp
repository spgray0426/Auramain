// Copyright Druid Mechanics

/*
 * AuraFireBall.cpp
 * 파이어볼 투사체 구현
 *
 * FireBlast 스킬에서 사용되는 특수 투사체입니다:
 * - 외부로 퍼져나가는 타임라인 기반 이동
 * - 충돌 시 폭발 GameplayCue 실행
 * - 기본 투사체와 달리 충돌해도 바로 파괴되지 않음
 */

#include "Actor/AuraFireBall.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayCueManager.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"

/** BeginPlay - 외부로 퍼져나가는 타임라인 시작 */
void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

/**
 * 스피어 오버랩 - 유효한 타겟에 데미지 적용
 * 부모 클래스와 달리 충돌해도 파괴되지 않음 (복귀 후 폭발)
 */
void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
	}
}

/** 충돌 처리 - FireBlast GameplayCue 실행 (폭발 이펙트/사운드) */
void AAuraFireBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParams;
		CueParams.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParams);
	}

	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bHit = true;
}
