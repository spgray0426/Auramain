// Copyright Druid Mechanics

/*
 * MMC_MaxMana.cpp
 * 최대 마나 계산 Modifier Magnitude Calculation 구현
 *
 * 캐릭터의 최대 마나를 계산합니다:
 * - 기본값: 50
 * - 지능(Intelligence) 1당 +2.5
 * - 레벨당 +15
 * - 공식: 50 + 2.5 * Intelligence + 15 * Level
 */

#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

/** 생성자 - Intelligence 속성 캡처 정의 */
UMMC_MaxMana::UMMC_MaxMana()
{
	IntDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntDef);
}

/**
 * 최대 마나 계산
 * Intelligence와 레벨을 기반으로 최대 마나 반환
 */
float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// 소스와 타겟의 태그 수집
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Int = 0.f;
	GetCapturedAttributeMagnitude(IntDef, Spec, EvaluationParameters, Int);
	Int = FMath::Max<float>(Int, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}
	
	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}
