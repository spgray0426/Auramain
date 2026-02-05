// Copyright Druid Mechanics

/**
 * AuraUserWidget.cpp
 *
 * 커스텀 사용자 위젯 베이스 클래스 구현 파일
 *
 * 모든 게임 내 UI 위젯의 기본 클래스입니다.
 * 위젯 컨트롤러와의 연결을 관리합니다.
 *
 * 주요 기능:
 * - 위젯 컨트롤러 설정 및 저장
 * - 컨트롤러 설정 완료 시 블루프린트 이벤트 호출
 *
 * 사용 방법:
 * 1. 블루프린트에서 이 클래스를 상속
 * 2. WidgetControllerSet 이벤트를 오버라이드하여 데이터 바인딩 설정
 */

#include "UI/Widget/AuraUserWidget.h"

/**
 * 위젯 컨트롤러 설정
 * @param InWidgetController - 설정할 위젯 컨트롤러
 *
 * 위젯 컨트롤러를 저장하고 블루프린트에서 구현 가능한 이벤트를 호출합니다.
 * 블루프린트에서 이 이벤트를 받아 델리게이트 바인딩 등을 수행합니다.
 */
void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	// 블루프린트 구현 가능 이벤트 호출
	WidgetControllerSet();
}
