// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

/**
 * EEffectApplicationPolicy
 * 이펙트 적용 정책 - 언제 이펙트를 적용할지 결정
 */
UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,      // 오버랩 시작 시 적용
	ApplyOnEndOverlap,   // 오버랩 종료 시 적용
	DoNotApply           // 적용하지 않음
};

/**
 * EEffectRemovalPolicy
 * 이펙트 제거 정책 - 언제 이펙트를 제거할지 결정
 */
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,  // 오버랩 종료 시 제거
	DoNotRemove          // 제거하지 않음
};

/**
 * AAuraEffectActor
 * 이펙트 액터 - 월드에 배치하여 플레이어에게 버프/디버프를 적용합니다.
 * 포션, 함정, 버프 아이템 등으로 사용됩니다.
 */
UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;

	// === 아이템 움직임 관련 ===
	// 계산된 위치
	UPROPERTY(BlueprintReadWrite)
	FVector CalculatedLocation;

	// 계산된 회전
	UPROPERTY(BlueprintReadWrite)
	FRotator CalculatedRotation;

	// 회전 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bRotates = false;

	// 회전 속도 (도/초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float RotationRate = 45.f;

	// 사인파 움직임 여부 (위아래로 부유)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	bool bSinusoidalMovement = false;

	// 사인파 움직임 시작
	UFUNCTION(BlueprintCallable)
	void StartSinusoidalMovement();

	// 회전 시작
	UFUNCTION(BlueprintCallable)
	void StartRotation();

	// 사인파 진폭
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SineAmplitude = 1.f;

	// 사인파 주기 상수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	float SinePeriodConstant = 1.f;

	// 초기 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup Movement")
	FVector InitialLocation;

	// === 이펙트 적용 관련 ===
	// 대상에게 이펙트 적용
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	// 오버랩 시작 시 호출
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	// 오버랩 종료 시 호출
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	// 이펙트 적용 후 액터 파괴 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bDestroyOnEffectApplication = false;

	// 적에게도 이펙트 적용 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	bool bApplyEffectsToEnemies = false;

	// 즉시 적용 이펙트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

	// 즉시 이펙트 적용 정책
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// 지속 시간 이펙트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	// 지속 이펙트 적용 정책
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// 무한 지속 이펙트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	// 무한 이펙트 적용 정책
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	// 무한 이펙트 제거 정책
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	// 활성 이펙트 핸들 맵
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

	// 액터 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Applied Effects")
	float ActorLevel = 1.f;

private:

	// 실행 시간
	float RunningTime = 0.f;

	// 아이템 움직임 처리
	void ItemMovement(float DeltaTime);
};