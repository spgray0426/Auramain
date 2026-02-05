#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

/**
 * FDamageEffectParams
 * 데미지 이펙트 파라미터 구조체
 * 데미지 적용에 필요한 모든 정보를 포함합니다.
 */
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams(){}

	// 월드 컨텍스트
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	// 데미지 게임플레이 이펙트 클래스
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	// 공격자의 ASC
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	// 대상의 ASC
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	// 기본 데미지
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	// 어빌리티 레벨
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	// 데미지 타입 (화염, 번개, 비전, 물리)
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	// === 디버프 파라미터 ===
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;       // 디버프 발생 확률

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;       // 디버프 틱당 데미지

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;     // 디버프 지속 시간

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;    // 디버프 틱 주기

	// === 사망 임펄스 ===
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;  // 사망 시 물리 힘 크기

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;  // 사망 시 물리 힘 방향

	// === 넉백 파라미터 ===
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;    // 넉백 힘 크기

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;            // 넉백 발생 확률

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;   // 넉백 힘 벡터

	// === 범위 데미지 파라미터 ===
	UPROPERTY(BlueprintReadWrite)
	bool bIsRadialDamage = false;           // 범위 데미지 여부

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;    // 내부 반경 (최대 데미지)

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;    // 외부 반경 (데미지 감소)

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;   // 범위 데미지 중심점

};

/**
 * FAuraGameplayEffectContext
 * 확장된 게임플레이 이펙트 컨텍스트
 * 치명타, 막기, 디버프, 넉백, 범위 데미지 등의 추가 정보를 포함합니다.
 * 네트워크 복제를 지원합니다.
 */
USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	// === Getter 함수 ===
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockedHit () const { return bIsBlockedHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

	// === Setter 함수 ===
	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }
	void SetIsSuccessfulDebuff(bool bInIsDebuff) { bIsSuccessfulDebuff = bInIsDebuff; }
	void SetDebuffDamage(float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse; }
	void SetKnockbackForce(const FVector& InForce) { KnockbackForce = InForce; }
	void SetIsRadialDamage(bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }

	// 직렬화에 사용되는 실제 구조체 반환 (서브클래스는 이를 오버라이드해야 함)
	virtual UScriptStruct* GetScriptStruct() const
	{
		return FGameplayEffectContext::StaticStruct();
	}

	// 컨텍스트 복사본 생성 (이후 수정을 위한 복제)
	virtual FGameplayEffectContext* Duplicate() const
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// 히트 결과 깊은 복사
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	// 커스텀 네트워크 직렬화 (서브클래스는 이를 오버라이드해야 함)
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	// === 공격 상태 ===
	UPROPERTY()
	bool bIsBlockedHit = false;         // 막기 성공 여부

	UPROPERTY()
	bool bIsCriticalHit = false;        // 치명타 여부

	// === 디버프 정보 ===
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;   // 디버프 성공 여부

	UPROPERTY()
	float DebuffDamage = 0.f;           // 디버프 틱당 데미지

	UPROPERTY()
	float DebuffDuration = 0.f;         // 디버프 지속 시간

	UPROPERTY()
	float DebuffFrequency = 0.f;        // 디버프 틱 주기

	TSharedPtr<FGameplayTag> DamageType;    // 데미지 타입

	// === 물리 힘 ===
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;     // 사망 임펄스

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;   // 넉백 힘

	// === 범위 데미지 ===
	UPROPERTY()
	bool bIsRadialDamage = false;       // 범위 데미지 여부

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;    // 내부 반경

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;    // 외부 반경

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;   // 범위 데미지 중심점
};

/**
 * FAuraGameplayEffectContext 타입 특성
 * 네트워크 직렬화와 복사 기능을 활성화합니다.
 */
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,   // 네트워크 직렬화 지원
		WithCopy = true             // 복사 지원
	};
};