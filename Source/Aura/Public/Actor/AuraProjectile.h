// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

/**
 * AAuraProjectile
 * 투사체 액터 - 스킬에서 발사되는 투사체를 표현합니다.
 * 이동, 충돌, 데미지 처리 등을 담당합니다.
 */
UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

	// 투사체 이동 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	// 데미지 이펙트 파라미터 (생성 시 설정)
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	// 유도 타겟 씬 컴포넌트
	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;

	// 충돌 처리
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();
	virtual void Destroyed() override;

	// 구체 오버랩 이벤트
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 충돌 감지용 구체 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;

	// 유효한 오버랩인지 확인 (아군/적군 체크 등)
	bool IsValidOverlap(AActor* OtherActor);

	// 이미 충돌했는지 여부
	bool bHit = false;

	// 루핑 사운드 컴포넌트
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
private:

	// 자동 소멸 시간 (초)
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	// 충돌 시 나이아가라 이펙트
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	// 충돌 사운드
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	// 비행 중 루핑 사운드
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
};
