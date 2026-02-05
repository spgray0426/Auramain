// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

/**
 * UMMC_MaxMana
 * 최대 마나 계산 클래스
 * 지능(Intelligence) 속성을 기반으로 최대 마나를 계산합니다.
 * 공식: MaxMana = 50 + (Intelligence * 2.5)
 */
UCLASS()
class AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxMana();

	// 최대 마나 기본 크기 계산
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition IntDef;       // 지능 속성 캡처 정의
};
