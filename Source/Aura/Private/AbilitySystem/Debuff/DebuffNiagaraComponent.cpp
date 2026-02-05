// Copyright Druid Mechanics

/*
 * DebuffNiagaraComponent.cpp
 * 디버프 나이아가라 컴포넌트 구현
 *
 * 디버프 상태를 시각적으로 표시하는 나이아가라 이펙트입니다:
 * - 화상, 기절 등의 디버프 태그 감지
 * - 태그 추가/제거에 따라 이펙트 활성화/비활성화
 * - ASC 등록 대기 기능 (지연 초기화 지원)
 */

#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

/** 생성자 - 자동 활성화 비활성화 */
UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
}

/**
 * BeginPlay - 디버프 태그 이벤트 등록
 * ASC가 아직 없으면 등록 대기
 */
void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	if (UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if (CombatInterface)
	{
		// ASC가 나중에 등록될 때 콜백
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UDebuffNiagaraComponent::DebuffTagChanged);
		});
	}
}

/**
 * 디버프 태그 변경 콜백
 * 태그가 추가되고 오너가 살아있으면 이펙트 활성화, 아니면 비활성화
 */
void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	const bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());

	if (NewCount > 0 && bOwnerValid && bOwnerAlive)
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}
