// Copyright Druid Mechanics

/**
 * LoadScreenHUD.cpp
 *
 * 로드 화면(타이틀/메인 메뉴) HUD 클래스 구현 파일
 *
 * 게임 시작 시 표시되는 로드 화면을 관리합니다.
 * MVVM(Model-View-ViewModel) 패턴을 사용합니다.
 *
 * 구성 요소:
 * - LoadScreenWidget: 뷰(View) - UI 표시
 * - MVVM_LoadScreen: 뷰모델(ViewModel) - 로직 및 데이터 바인딩
 * - LoadScreenSaveGame: 모델(Model) - 실제 저장 데이터
 *
 * 기능:
 * - 세이브 슬롯 목록 표시 (최대 3개)
 * - 새 게임 시작
 * - 저장된 게임 불러오기
 * - 세이브 슬롯 삭제
 */

#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

/**
 * BeginPlay
 * 로드 화면 초기화
 *
 * 뷰모델과 위젯을 생성하고 초기 데이터를 로드합니다.
 */
void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	// 뷰모델 생성 및 슬롯 초기화
	LoadScreenViewModel = NewObject<UMVVM_LoadScreen>(this, LoadScreenViewModelClass);
	LoadScreenViewModel->InitializeLoadSlots();

	// 위젯 생성 및 뷰포트에 추가
	LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	LoadScreenWidget->AddToViewport();
	// 블루프린트에서 추가 초기화 수행 (뷰모델 바인딩 등)
	LoadScreenWidget->BlueprintInitializeWidget();

	// 저장된 데이터 로드
	LoadScreenViewModel->LoadData();
}
