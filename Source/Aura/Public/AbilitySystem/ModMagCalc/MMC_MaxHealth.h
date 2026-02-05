// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * UMMC_MaxHealth
 * 최대 체력 계산 클래스
 * 활력(Vigor) 속성을 기반으로 최대 체력을 계산합니다.
 * 공식: MaxHealth = 80 + (Vigor * 2.5)
 */
UCLASS()
class AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxHealth();

	// 최대 체력 기본 크기 계산
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition VigorDef;     // 활력 속성 캡처 정의
};
