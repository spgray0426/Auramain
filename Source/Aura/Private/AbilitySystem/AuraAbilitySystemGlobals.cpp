// Copyright Druid Mechanics

/*
 * AuraAbilitySystemGlobals.cpp
 * Aura 프로젝트의 어빌리티 시스템 글로벌 설정 구현
 *
 * 이 클래스는 UAbilitySystemGlobals를 확장하여 커스텀 게임플레이 이펙트 컨텍스트를 사용합니다.
 * DefaultGame.ini에서 AbilitySystemGlobalsClassName을 이 클래스로 지정해야 합니다.
 */

#include "AbilitySystem/AuraAbilitySystemGlobals.h"

#include "AuraAbilityTypes.h"

/**
 * 게임플레이 이펙트 컨텍스트 생성
 * 기본 FGameplayEffectContext 대신 FAuraGameplayEffectContext를 생성하여
 * 크리티컬 히트, 디버프, 광역 데미지 등의 추가 정보를 저장할 수 있게 합니다.
 *
 * @return 새로 생성된 FAuraGameplayEffectContext 포인터
 */
FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext();
}
