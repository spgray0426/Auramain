// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * UAuraBeamSpell
 * 빔 형태의 스킬 기본 클래스
 * 대상을 향해 빔을 발사하고, 주변 적들에게 연쇄 공격을 가합니다.
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	// 마우스 커서 히트 정보를 저장
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& HitResult);

	// 스킬 사용자의 정보를 저장
	UFUNCTION(BlueprintCallable)
	void StoreOwnerVariables();

	// 첫 번째 타겟을 추적
	UFUNCTION(BlueprintCallable)
	void TraceFirstTarget(const FVector& BeamTargetLocation);

	// 추가 타겟들을 찾아 저장
	UFUNCTION(BlueprintCallable)
	void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);

	// 주 타겟이 사망했을 때 호출 (블루프린트에서 구현)
	UFUNCTION(BlueprintImplementableEvent)
	void PrimaryTargetDied(AActor* DeadActor);

	// 추가 타겟이 사망했을 때 호출 (블루프린트에서 구현)
	UFUNCTION(BlueprintImplementableEvent)
	void AdditionalTargetDied(AActor* DeadActor);
protected:

	// 마우스 커서가 히트한 위치
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	FVector MouseHitLocation;

	// 마우스 커서가 히트한 액터
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<AActor> MouseHitActor;

	// 스킬 사용자의 플레이어 컨트롤러
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	// 스킬 사용자 캐릭터
	UPROPERTY(BlueprintReadWrite, Category = "Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	// 최대 연쇄 공격 대상 개수
	UPROPERTY(EditDefaultsOnly, Category = "Beam")
	int32 MaxNumShockTargets = 5;
};
