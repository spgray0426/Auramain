// Copyright Druid Mechanics

/*
 * AuraGameplayAbility.cpp
 * Aura 프로젝트의 기본 게임플레이 어빌리티 구현
 *
 * 모든 커스텀 어빌리티의 기본 클래스로, 공통 기능을 제공합니다:
 * - 어빌리티 설명 텍스트 생성 (리치 텍스트 지원)
 * - 마나 비용 조회
 * - 쿨다운 시간 조회
 */

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

/**
 * 어빌리티 설명 텍스트 반환 (현재 레벨)
 * 자식 클래스에서 오버라이드하여 구체적인 설명을 제공합니다.
 */
FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum LoremIpsum", Level);
}

/** 다음 레벨 설명 텍스트 반환 */
FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), Level);
}

/** 잠긴 상태 설명 텍스트 반환 (해금 필요 레벨 표시) */
FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d</>"), Level);
}

/**
 * 마나 비용 조회
 * CostGameplayEffect에서 Mana 속성 modifier를 찾아 비용을 반환합니다.
 */
float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

/**
 * 쿨다운 시간 조회
 * CooldownGameplayEffect의 Duration을 반환합니다.
 */
float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	float Cooldown = 0.f;
	if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}
