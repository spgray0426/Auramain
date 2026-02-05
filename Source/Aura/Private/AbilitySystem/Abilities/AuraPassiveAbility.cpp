// Copyright Druid Mechanics

/*
 * AuraPassiveAbility.cpp
 * 패시브 어빌리티 기본 클래스 구현
 *
 * 자동으로 활성화되어 지속적인 효과를 제공하는 패시브 스킬입니다.
 * 장착 해제 시 비활성화 델리게이트를 통해 종료됩니다.
 */

#include "AbilitySystem/Abilities/AuraPassiveAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"

/**
 * 패시브 어빌리티 활성화
 * ASC의 비활성화 델리게이트에 바인딩하여 장착 해제 시 종료되도록 합니다.
 */
void UAuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->DeactivatePassiveAbility.AddUObject(this, &UAuraPassiveAbility::ReceiveDeactivate);
	}
}

/**
 * 비활성화 요청 수신
 * 어빌리티 태그가 일치하면 어빌리티를 종료합니다.
 */
void UAuraPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
