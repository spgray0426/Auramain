// Copyright Druid Mechanics

/**
 * AttributeMenuWidgetController.cpp
 *
 * 속성 메뉴 위젯 컨트롤러 클래스 구현 파일
 *
 * 캐릭터 속성 메뉴 UI의 데이터를 관리합니다.
 * 플레이어가 속성 포인트를 사용하여 능력치를 업그레이드할 수 있습니다.
 *
 * 기능:
 * - 1차 속성 (Strength, Intelligence, Resilience, Vigor) 표시
 * - 2차 속성 (Armor, ArmorPen, BlockChance 등) 표시
 * - 속성 포인트 잔여량 표시
 * - 속성 업그레이드 요청 처리
 *
 * 데이터 흐름:
 * ASC(속성 변경) -> 위젯 컨트롤러 -> 델리게이트 -> UI 위젯
 */

#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

/**
 * 의존성에 콜백 바인딩
 * 속성 변경 및 속성 포인트 변경 이벤트를 구독합니다.
 */
void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AttributeInfo);

	// 모든 속성에 대해 변경 콜백 등록
	for (auto& Pair : GetAuraAS()->TagsToAttributes)
	{
		// 속성 값 변경 시 해당 속성 정보를 UI에 브로드캐스트
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
		[this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastAttributeInfo(Pair.Key, Pair.Value());
		}
	);
	}

	// 속성 포인트 변경 콜백
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

/**
 * 초기값 브로드캐스트
 * 메뉴 열 때 모든 속성의 현재 값을 UI에 전달합니다.
 */
void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// 모든 속성 정보 브로드캐스트
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}

	// 속성 포인트 잔여량 브로드캐스트
	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
}

/**
 * 속성 업그레이드 요청
 * @param AttributeTag - 업그레이드할 속성의 게임플레이 태그
 *
 * 서버 RPC를 통해 ASC에 속성 업그레이드를 요청합니다.
 */
void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}

/**
 * 속성 정보 브로드캐스트
 * @param AttributeTag - 속성의 게임플레이 태그
 * @param Attribute - 속성 참조
 *
 * AttributeInfo 데이터 에셋에서 속성 정보를 조회하고
 * 현재 값을 설정하여 UI에 브로드캐스트합니다.
 */
void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	// 데이터 에셋에서 속성 정보 조회 (이름, 설명, 아이콘 등)
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	// 현재 속성 값 설정
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	// 델리게이트로 브로드캐스트
	AttributeInfoDelegate.Broadcast(Info);
}
