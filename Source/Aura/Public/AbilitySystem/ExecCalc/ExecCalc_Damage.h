// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"

/**
 * UExecCalc_Damage
 * 데미지 계산 실행 클래스
 * 데미지 적용 시 복잡한 계산을 처리합니다:
 * - 방어력과 방어구 관통 계산
 * - 막기 확률 판정
 * - 치명타 확률 및 데미지 계산
 * - 저항과 데미지 타입 계산
 * - 디버프 판정 및 적용
 * - 범위 데미지 감소 계산
 */
UCLASS()
class AURA_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UExecCalc_Damage();

	// 디버프 발생 여부 판정
	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                     const FGameplayEffectSpec& Spec,
	                     FAggregatorEvaluateParameters EvaluationParameters,
	                     const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;

	// 실제 데미지 계산 및 적용 실행
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
