// Copyright Druid Mechanics

/**
 * AuraHUD.cpp
 *
 * 게임 플레이 중 표시되는 HUD 클래스 구현 파일
 *
 * AHUD를 상속받아 게임 UI 시스템의 중앙 관리자 역할을 합니다.
 * - 오버레이 위젯(체력바, 마나바, XP바 등) 관리
 * - 위젯 컨트롤러 생성 및 연결
 * - MVC 패턴에서 뷰(View)와 컨트롤러를 연결하는 역할
 *
 * 위젯 컨트롤러 종류:
 * - OverlayWidgetController: 메인 게임 UI (체력, 마나, XP)
 * - AttributeMenuWidgetController: 속성 메뉴
 * - SpellMenuWidgetController: 스펠/어빌리티 메뉴
 */

#include "UI/HUD/AuraHUD.h"

#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"

/**
 * 오버레이 위젯 컨트롤러 가져오기 (Lazy Initialization)
 * @param WCParams - 위젯 컨트롤러 초기화 파라미터
 * @return 오버레이 위젯 컨트롤러 인스턴스
 *
 * 싱글톤 패턴으로 구현되어 처음 호출 시에만 생성됩니다.
 */
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		// 위젯 컨트롤러 생성 및 초기화
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		// ASC 등의 의존성에 콜백 바인딩
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

/**
 * 속성 메뉴 위젯 컨트롤러 가져오기
 * @param WCParams - 위젯 컨트롤러 초기화 파라미터
 * @return 속성 메뉴 위젯 컨트롤러 인스턴스
 */
UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

/**
 * 스펠 메뉴 위젯 컨트롤러 가져오기
 * @param WCParams - 위젯 컨트롤러 초기화 파라미터
 * @return 스펠 메뉴 위젯 컨트롤러 인스턴스
 */
USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (SpellMenuWidgetController == nullptr)
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}
	return SpellMenuWidgetController;
}

/**
 * 오버레이 초기화
 * @param PC - 플레이어 컨트롤러
 * @param PS - 플레이어 상태
 * @param ASC - 어빌리티 시스템 컴포넌트
 * @param AS - 속성 집합
 *
 * 게임 시작 시 플레이어 컨트롤러에서 호출됩니다.
 * 오버레이 위젯을 생성하고 위젯 컨트롤러와 연결한 후 뷰포트에 추가합니다.
 */
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 블루프린트에서 클래스가 설정되었는지 확인
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	// 오버레이 위젯 생성
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	// 위젯 컨트롤러 파라미터 설정 및 컨트롤러 가져오기
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	// 위젯에 컨트롤러 설정 및 초기값 브로드캐스트
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();

	// 뷰포트에 위젯 추가
	Widget->AddToViewport();
}

