// Copyright Druid Mechanics

/**
 * AuraWidgetController.cpp
 *
 * 위젯 컨트롤러 베이스 클래스 구현 파일
 *
 * MVC/MVVM 패턴에서 컨트롤러 역할을 하는 클래스입니다.
 * 모델(ASC, AttributeSet)과 뷰(Widget)를 연결합니다.
 *
 * 주요 역할:
 * - 데이터 의존성 저장 (PC, PS, ASC, AS)
 * - 초기값 브로드캐스트
 * - ASC의 속성 변경 콜백 바인딩
 * - 어빌리티 정보 브로드캐스트
 *
 * 서브클래스:
 * - OverlayWidgetController
 * - AttributeMenuWidgetController
 * - SpellMenuWidgetController
 */

#include "UI/WidgetController/AuraWidgetController.h"

#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

/**
 * 위젯 컨트롤러 파라미터 설정
 * @param WCParams - 초기화에 필요한 파라미터 구조체
 *
 * 플레이어 컨트롤러, 플레이어 상태, ASC, AttributeSet 참조를 저장합니다.
 */
void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

/**
 * 초기값 브로드캐스트
 * 서브클래스에서 오버라이드하여 초기 UI 값을 설정합니다.
 */
void UAuraWidgetController::BroadcastInitialValues()
{
	// 서브클래스에서 구현
}

/**
 * 의존성에 콜백 바인딩
 * 서브클래스에서 오버라이드하여 ASC 등의 이벤트를 구독합니다.
 */
void UAuraWidgetController::BindCallbacksToDependencies()
{
	// 서브클래스에서 구현
}

/**
 * 어빌리티 정보 브로드캐스트
 * 현재 보유한 모든 어빌리티 정보를 UI에 전달합니다.
 *
 * 시작 어빌리티가 부여되었는지 확인 후,
 * 각 어빌리티의 태그, 입력 태그, 상태 태그를 포함한 정보를 브로드캐스트합니다.
 */
void UAuraWidgetController::BroadcastAbilityInfo()
{
	// 시작 어빌리티가 아직 부여되지 않았으면 리턴
	if (!GetAuraASC()->bStartupAbilitiesGiven) return;

	// 모든 어빌리티를 순회하며 정보 브로드캐스트
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		// 어빌리티 태그로 정보 조회
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		// 입력 태그 및 상태 태그 추가
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = AuraAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		// 델리게이트로 브로드캐스트
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetAuraASC()->ForEachAbility(BroadcastDelegate);
}

/**
 * Aura 플레이어 컨트롤러 캐스팅 (캐싱 적용)
 */
AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

/**
 * Aura 플레이어 상태 캐스팅 (캐싱 적용)
 */
AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPlayerState == nullptr)
	{
		AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
	}
	return AuraPlayerState;
}

/**
 * Aura 어빌리티 시스템 컴포넌트 캐스팅 (캐싱 적용)
 */
UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraAbilitySystemComponent;
}

/**
 * Aura 속성 집합 캐스팅 (캐싱 적용)
 */
UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
