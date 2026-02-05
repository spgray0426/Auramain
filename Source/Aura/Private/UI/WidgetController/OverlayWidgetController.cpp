// Copyright Druid Mechanics

/**
 * OverlayWidgetController.cpp
 *
 * 오버레이 위젯 컨트롤러 클래스 구현 파일
 *
 * 게임 플레이 중 항상 표시되는 메인 HUD의 데이터를 관리합니다.
 * 체력바, 마나바, XP바, 레벨, 메시지 등의 정보를 UI에 전달합니다.
 *
 * 관리하는 UI 요소:
 * - 체력(Health) / 최대 체력
 * - 마나(Mana) / 최대 마나
 * - 경험치(XP) 퍼센트
 * - 플레이어 레벨
 * - 게임플레이 메시지 (물약 획득 등)
 * - 어빌리티 슬롯 정보
 */

#include "UI/WidgetController/OverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

/**
 * 초기값 브로드캐스트
 * HUD 초기화 시 현재 체력, 마나 값을 UI에 전달합니다.
 */
void UOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

/**
 * 의존성에 콜백 바인딩
 * 속성 변경, XP 변경, 레벨 변경, 어빌리티 이벤트 등을 구독합니다.
 */
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// XP 변경 콜백
	GetAuraPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);

	// 레벨 변경 콜백
	GetAuraPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel, bool bLevelUp)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
		}
	);

	// 체력 변경 콜백
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// 최대 체력 변경 콜백
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

	// 마나 변경 콜백
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			}
		);

	// 최대 마나 변경 콜백
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			}
		);

	if (GetAuraASC())
	{
		// 어빌리티 장착 이벤트 콜백
		GetAuraASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);

		// 시작 어빌리티가 이미 부여되었으면 즉시 브로드캐스트
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			// 아직 부여되지 않았으면 델리게이트에 바인딩
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		// 게임플레이 이펙트 태그 콜백 (메시지 표시용)
		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// "Message" 태그로 시작하는 태그만 처리
					// 예: "Message.HealthPotion".MatchesTag("Message") = True
					//     "Message".MatchesTag("Message.HealthPotion") = False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						// 데이터 테이블에서 해당 태그의 메시지 정보 조회
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}
}

/**
 * XP 변경 처리
 * @param NewXP - 새로운 총 경험치
 *
 * 현재 레벨에서의 XP 진행률을 계산하여 UI에 전달합니다.
 */
void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

	// 현재 XP에 해당하는 레벨 계산
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		// 현재 레벨의 필요 경험치
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		// 이전 레벨의 필요 경험치
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		// 현재 레벨 구간의 총 필요 경험치
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		// 현재 레벨 구간에서 획득한 경험치
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		// XP바 퍼센트 계산 (0.0 ~ 1.0)
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

/**
 * 어빌리티 장착 처리
 * @param AbilityTag - 장착된 어빌리티 태그
 * @param Status - 어빌리티 상태 태그
 * @param Slot - 장착된 슬롯 태그
 * @param PreviousSlot - 이전 슬롯 태그 (재장착 시)
 *
 * 슬롯 변경 시 이전 슬롯을 비우고 새 슬롯에 어빌리티 정보를 설정합니다.
 */
void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// 이전 슬롯 정보 초기화 (이미 장착된 스펠을 다른 슬롯으로 옮길 때)
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	// 이전 슬롯이 유효한 경우 빈 정보 브로드캐스트
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	// 새 슬롯에 어빌리티 정보 설정
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
