// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

/**
 * UAuraAbilitySystemGlobals
 * Aura GAS 글로벌 설정 클래스
 * 커스텀 게임플레이 이펙트 컨텍스트(FAuraGameplayEffectContext)를 할당합니다.
 * 치명타, 막기, 디버프 등의 추가 정보를 포함하는 확장된 컨텍스트를 제공합니다.
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	// 커스텀 게임플레이 이펙트 컨텍스트 할당
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
