// Copyright Druid Mechanics

/**
 * SpellMenuWidgetController.cpp
 *
 * 스펠 메뉴 위젯 컨트롤러 클래스 구현 파일
 *
 * 스펠/어빌리티 메뉴 UI의 데이터와 로직을 관리합니다.
 * 플레이어가 스펠 포인트를 사용하여 어빌리티를 해금하고 장착합니다.
 *
 * 기능:
 * - 어빌리티 목록 표시 (잠금/해금/장착 상태)
 * - 스펠 포인트로 어빌리티 해금/업그레이드
 * - 어빌리티 슬롯 장착/재배치
 * - 어빌리티 설명 표시
 *
 * 어빌리티 상태:
 * - Locked: 레벨 조건 미달, 해금 불가
 * - Eligible: 레벨 조건 충족, 스펠 포인트로 해금 가능
 * - Unlocked: 해금됨, 장착 가능
 * - Equipped: 슬롯에 장착됨
 */

#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/AuraPlayerState.h"

/**
 * 초기값 브로드캐스트
 * 메뉴 열 때 어빌리티 정보와 스펠 포인트를 UI에 전달합니다.
 */
void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

/**
 * 의존성에 콜백 바인딩
 * 어빌리티 상태 변경, 장착, 스펠 포인트 변경 이벤트를 구독합니다.
 */
void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	// 어빌리티 상태 변경 콜백 (해금, 업그레이드 등)
	GetAuraASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
	{
		// 현재 선택된 어빌리티의 상태가 변경된 경우 UI 업데이트
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
		}

		// 어빌리티 정보 업데이트 브로드캐스트
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});

	// 어빌리티 장착 콜백
	GetAuraASC()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	// 스펠 포인트 변경 콜백
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChanged.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;

		// 버튼 상태 업데이트
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
	});
}

/**
 * 스펠 글로브 선택 (어빌리티 아이콘 클릭)
 * @param AbilityTag - 선택된 어빌리티 태그
 *
 * 어빌리티 정보를 조회하고 버튼 상태를 결정합니다.
 */
void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	// 장착 대기 중이었다면 취소
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	// 어빌리티 상태 확인
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = AbilitySpec != nullptr;

	// 유효하지 않은 어빌리티는 잠금 상태로 처리
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	// 선택 정보 저장 및 UI 업데이트
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

/**
 * 포인트 사용 버튼 클릭
 * 선택된 어빌리티에 스펠 포인트를 사용합니다.
 */
void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

/**
 * 글로브 선택 해제
 * 선택을 초기화하고 장착 대기 상태를 취소합니다.
 */
void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}

	// 선택 정보 초기화
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

/**
 * 장착 버튼 클릭
 * 장착 대기 모드로 전환하고 슬롯 선택을 기다립니다.
 */
void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

	// 장착 대기 모드 시작 (슬롯 하이라이트 등)
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	// 이미 장착된 어빌리티의 경우 현재 슬롯 저장
	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetSlotFromAbilityTag(SelectedAbility.Ability);
	}
}

/**
 * 슬롯 글로브 클릭 (하단 어빌리티 슬롯)
 * @param SlotTag - 클릭된 슬롯 태그
 * @param AbilityType - 슬롯의 어빌리티 타입 (공격/패시브)
 *
 * 장착 대기 중일 때 해당 슬롯에 어빌리티를 장착합니다.
 */
void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;

	// 어빌리티 타입이 슬롯과 일치하는지 확인
	// (공격 스펠을 패시브 슬롯에 장착하거나 그 반대 방지)
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

	// 서버에 장착 요청
	GetAuraASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

/**
 * 어빌리티 장착 완료 처리
 * @param AbilityTag - 장착된 어빌리티 태그
 * @param Status - 어빌리티 상태
 * @param Slot - 장착된 슬롯
 * @param PreviousSlot - 이전 슬롯 (재배치 시)
 */
void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	bWaitingForEquipSelection = false;

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// 이전 슬롯 비우기 (재배치 시)
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// 새 슬롯에 어빌리티 정보 설정
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);

	// 장착 대기 모드 종료
	StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
	SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
}

/**
 * 버튼 활성화 여부 결정
 * @param AbilityStatus - 어빌리티 상태 태그
 * @param SpellPoints - 현재 스펠 포인트
 * @param bShouldEnableSpellPointsButton - [출력] 포인트 사용 버튼 활성화 여부
 * @param bShouldEnableEquipButton - [출력] 장착 버튼 활성화 여부
 *
 * 어빌리티 상태와 스펠 포인트에 따라 버튼 활성화를 결정합니다.
 */
void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;

	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		// 장착됨: 장착 가능(재배치), 포인트가 있으면 업그레이드 가능
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		// 해금 가능: 포인트가 있으면 해금 가능
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		// 해금됨: 장착 가능, 포인트가 있으면 업그레이드 가능
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	// Locked 상태: 모두 비활성화 (기본값)
}
