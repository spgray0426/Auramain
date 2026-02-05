// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "PassiveNiagaraComponent.generated.h"

class UAuraAbilitySystemComponent;

/**
 * UPassiveNiagaraComponent
 * 패시브 나이아가라 컴포넌트
 * 패시브 어빌리티가 활성화될 때 자동으로 활성화/비활성화되는 파티클 이펙트입니다.
 * 보호의 후광, 생명력 흡수, 마나 흡수 등의 패시브 스킬 시각 효과를 표시합니다.
 */
UCLASS()
class AURA_API UPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UPassiveNiagaraComponent();

	// 이 컴포넌트가 표시할 패시브 스킬 태그
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PassiveSpellTag;

protected:
	virtual void BeginPlay() override;

	// 패시브 활성화 콜백
	void OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate);

	// 패시브가 장착되어 있으면 활성화
	void ActivateIfEquipped(UAuraAbilitySystemComponent* AuraASC);
};
