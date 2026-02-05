// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class ULoadScreenSaveGame;

// 이펙트 에셋 태그 델리게이트 - 이펙트가 적용되었을 때 호출
DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
// 어빌리티 부여 델리게이트 - 어빌리티가 주어졌을 때 호출
DECLARE_MULTICAST_DELEGATE(FAbilitiesGiven);
// 각 어빌리티 순회 델리게이트
DECLARE_DELEGATE_OneParam(FForEachAbility, const FGameplayAbilitySpec&);
// 어빌리티 상태 변경 델리게이트 - 어빌리티 상태가 바뀔 때 호출
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChanged, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, int32 /*AbilityLevel*/);
// 어빌리티 장착 델리게이트 - 어빌리티가 슬롯에 장착될 때 호출
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquipped, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*Status*/, const FGameplayTag& /*Slot*/, const FGameplayTag& /*PrevSlot*/);
// 패시브 어빌리티 비활성화 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbility, const FGameplayTag& /*AbilityTag*/);
// 패시브 이펙트 활성화 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(FActivatePassiveEffect, const FGameplayTag& /*AbilityTag*/, bool /*bActivate*/);

/**
 * UAuraAbilitySystemComponent
 * Aura 프로젝트의 커스텀 어빌리티 시스템 컴포넌트
 * 어빌리티 관리, 입력 처리, 슬롯 시스템 등을 담당합니다.
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	// 어빌리티 액터 정보 설정 완료 처리
	void AbilityActorInfoSet();

	// 델리게이트들
	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	FAbilityStatusChanged AbilityStatusChanged;
	FAbilityEquipped AbilityEquipped;
	FDeactivatePassiveAbility DeactivatePassiveAbility;
	FActivatePassiveEffect ActivatePassiveEffect;

	// 저장 데이터로부터 캐릭터 어빌리티 추가
	void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData);
	// 캐릭터에 어빌리티들을 추가
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	// 캐릭터에 패시브 어빌리티들을 추가
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	// 시작 어빌리티가 이미 주어졌는지 여부
	bool bStartupAbilitiesGiven = false;

	// 어빌리티 입력 태그 눌림 처리
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	// 어빌리티 입력 태그 유지 처리
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	// 어빌리티 입력 태그 해제 처리
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	// 모든 어빌리티에 대해 델리게이트 실행
	void ForEachAbility(const FForEachAbility& Delegate);

	// 어빌리티 스펙으로부터 어빌리티 태그 가져오기
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	// 어빌리티 스펙으로부터 입력 태그 가져오기
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	// 어빌리티 스펙으로부터 상태 가져오기
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	// 어빌리티 태그로부터 상태 가져오기
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	// 어빌리티 태그로부터 슬롯 가져오기
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);
	// 슬롯이 비어있는지 확인
	bool SlotIsEmpty(const FGameplayTag& Slot);
	// 어빌리티가 특정 슬롯을 가지고 있는지 확인
	static bool AbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	// 어빌리티가 어떤 슬롯이든 가지고 있는지 확인
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	// 특정 슬롯을 가진 어빌리티 스펙 가져오기
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& Slot);
	// 패시브 어빌리티인지 확인
	bool IsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	// 어빌리티에 슬롯 할당
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);

	// 패시브 이펙트 활성화를 모든 클라이언트에 전달
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastActivatePassiveEffect(const FGameplayTag& AbilityTag, bool bActivate);

	// 어빌리티 태그로부터 어빌리티 스펙 가져오기
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);

	// 속성 업그레이드
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	// 서버에서 속성 업그레이드 처리
	UFUNCTION(Server, Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);

	// 레벨에 따라 어빌리티 상태 업데이트
	void UpdateAbilityStatuses(int32 Level);

	// 서버에서 주문 포인트 사용 처리
	UFUNCTION(Server, Reliable)
	void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

	// 서버에서 어빌리티 장착 처리
	UFUNCTION(Server, Reliable)
	void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);

	// 클라이언트에서 어빌리티 장착 처리
	UFUNCTION(Client, Reliable)
	void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

	// 어빌리티 태그로 설명 가져오기
	bool GetDescriptionsByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	// 어빌리티 스펙의 슬롯 클리어
	static void ClearSlot(FGameplayAbilitySpec* Spec);
	// 특정 슬롯의 모든 어빌리티 클리어
	void ClearAbilitiesOfSlot(const FGameplayTag& Slot);
	// 어빌리티가 특정 슬롯을 가지고 있는지 확인
	static bool AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot);
protected:

	// 어빌리티 활성화 복제 처리
	virtual void OnRep_ActivateAbilities() override;

	// 클라이언트에서 이펙트 적용 처리
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	// 클라이언트에서 어빌리티 상태 업데이트 처리
	UFUNCTION(Client, Reliable)
	void ClientUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel);
};
