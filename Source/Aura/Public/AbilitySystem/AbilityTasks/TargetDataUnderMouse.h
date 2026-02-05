// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

// 마우스 타겟 데이터 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * UTargetDataUnderMouse
 * 어빌리티 태스크 - 마우스 커서 아래의 타겟 데이터를 가져옵니다.
 * 클릭 기반 스킬에서 타겟 위치를 얻을 때 사용됩니다.
 */
UCLASS()
class AURA_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:

	// 마우스 아래 타겟 데이터를 가져오는 태스크 생성
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	// 유효한 데이터를 받았을 때 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:

	// 태스크 활성화
	virtual void Activate() override;

	// 마우스 커서 데이터 전송
	void SendMouseCursorData();

	// 타겟 데이터 복제 콜백
	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
