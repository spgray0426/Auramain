// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GameplayTagContainer.h"
#include "ActiveGameplayEffectHandle.h"
#include "WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;

// 쿨다운 변경 델리게이트 (남은 시간 전달)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 * UWaitCooldownChange
 * 쿨다운 변경 대기 비동기 태스크
 * 어빌리티의 쿨다운 시작과 종료를 감지하여 블루프린트에 알립니다.
 * UI 쿨다운 타이머 업데이트에 사용됩니다.
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class AURA_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	// 쿨다운 시작 델리게이트
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownStart;

	// 쿨다운 종료 델리게이트
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature CooldownEnd;

	// 쿨다운 변경 대기 태스크 생성
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	// 태스크 종료
	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	// 어빌리티 시스템 컴포넌트
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	// 감시할 쿨다운 태그
	FGameplayTag CooldownTag;

	// 쿨다운 태그 변경 콜백
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	// 액티브 이펙트 추가 콜백
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveEffectHandle);
};
