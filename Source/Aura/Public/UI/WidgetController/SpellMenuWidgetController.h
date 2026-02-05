// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

// 스펠 글로브 선택 델리게이트 (버튼 상태, 설명 텍스트 포함)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
// 장착 대기 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
// 스펠 재할당 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityTag);

/**
 * FSelectedAbility
 * 선택된 어빌리티 정보
 */
struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();  // 어빌리티 태그
	FGameplayTag Status = FGameplayTag();   // 어빌리티 상태 (잠김, 획득 가능, 잠금 해제, 장착됨)
};

/**
 * USpellMenuWidgetController
 * 스펠 메뉴 위젯 컨트롤러
 * 스펠(어빌리티) 메뉴 UI를 관리합니다.
 * 플레이어가 주문 포인트를 사용하여 어빌리티를 잠금 해제하고 업그레이드하며 장착할 수 있습니다.
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	// === 델리게이트 ===
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangedSignature SpellPointsChanged;           // 주문 포인트 변경 시

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;    // 스펠 글로브 선택 시

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;       // 장착 대기 시작

	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitingForEquipDelegate;    // 장착 대기 중지

	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReassignedSignature SpellGlobeReassignedDelegate;    // 스펠 재할당 시

	// === UI 상호작용 함수 ===

	// 스펠 글로브 선택
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	// 포인트 소비 버튼 클릭 (어빌리티 잠금 해제 또는 업그레이드)
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	// 글로브 선택 해제
	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

	// 장착 버튼 클릭
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	// 스펠 행 글로브 클릭 (슬롯 선택)
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

	// 어빌리티 장착 콜백
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

private:

	// 버튼 활성화 여부 판단
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton);

	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None,  FAuraGameplayTags::Get().Abilities_Status_Locked };  // 현재 선택된 어빌리티
	int32 CurrentSpellPoints = 0;               // 현재 주문 포인트
	bool bWaitingForEquipSelection = false;     // 장착 대기 중 여부
	FGameplayTag SelectedSlot;                  // 선택된 슬롯
};

