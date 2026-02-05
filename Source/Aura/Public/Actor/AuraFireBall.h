// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "AuraFireBall.generated.h"

/**
 * AAuraFireBall
 * 화염구 투사체
 * 화염 폭발 스킬에 사용되는 특수 투사체입니다.
 * 발사 후 일정 시간 동안 캐스터 주변을 맴돌다가 목표를 향해 날아갑니다.
 * 범위 데미지와 화상 디버프를 유발합니다.
 */
UCLASS()
class AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()
public:

	// 발사 타임라인 시작 (블루프린트에서 구현)
	UFUNCTION(BlueprintImplementableEvent)
	void StartOutgoingTimeline();

	// 돌아갈 액터 (캐스터)
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> ReturnToActor;

	// 폭발 데미지 파라미터
	UPROPERTY(BlueprintReadWrite)
	FDamageEffectParams ExplosionDamageParams;

protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 충돌 시 처리
	virtual void OnHit() override;
};
