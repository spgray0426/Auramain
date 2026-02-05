// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameplayTagContainer.h"
#include "DebuffNiagaraComponent.generated.h"

/**
 * UDebuffNiagaraComponent
 * 디버프 나이아가라 컴포넌트
 * 디버프 상태에 따라 자동으로 활성화/비활성화되는 파티클 이펙트입니다.
 * 화상, 기절 등의 디버프 시각 효과를 표시합니다.
 */
UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()
public:
	UDebuffNiagaraComponent();

	// 이 컴포넌트가 표시할 디버프 태그
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;

protected:
	virtual void BeginPlay() override;

	// 디버프 태그 변경 콜백 (디버프 시작/종료 시)
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
