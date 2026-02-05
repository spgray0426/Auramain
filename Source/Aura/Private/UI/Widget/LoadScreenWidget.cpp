// Copyright Druid Mechanics

/**
 * LoadScreenWidget.cpp
 *
 * 로드 화면 위젯 클래스 구현 파일
 *
 * 메인 메뉴/타이틀 화면의 UI 위젯입니다.
 * 세이브 슬롯 선택 및 게임 시작/로드 인터페이스를 제공합니다.
 *
 * 기능:
 * - 세이브 슬롯 목록 표시 (3개 슬롯)
 * - 새 게임 생성
 * - 저장된 게임 로드
 * - 세이브 삭제
 *
 * MVVM 패턴:
 * - 이 위젯은 뷰(View)로서 MVVM_LoadScreen 뷰모델과 바인딩됩니다.
 * - 블루프린트에서 BlueprintInitializeWidget()을 구현하여 바인딩 설정
 */

#include "UI/Widget/LoadScreenWidget.h"

