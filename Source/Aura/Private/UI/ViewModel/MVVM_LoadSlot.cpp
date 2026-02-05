// Copyright Druid Mechanics

/**
 * MVVM_LoadSlot.cpp
 *
 * 로드 슬롯 뷰모델 클래스 구현 파일
 *
 * 개별 세이브 슬롯의 데이터와 상태를 관리합니다.
 * MVVM 패턴의 필드 노티파이(Field Notify)를 사용하여
 * UI 바인딩을 자동화합니다.
 *
 * 관리하는 데이터:
 * - PlayerName: 플레이어 이름
 * - MapName: 현재 맵 이름
 * - PlayerLevel: 플레이어 레벨
 * - LoadSlotName: 슬롯 파일 이름
 * - SlotStatus: 슬롯 상태 (비어있음/사용중)
 *
 * UI 상호작용:
 * - SetWidgetSwitcherIndex: 슬롯 UI 상태 전환
 * - EnableSelectSlotButton: 선택 버튼 활성화/비활성화
 */

#include "UI/ViewModel/MVVM_LoadSlot.h"

/**
 * 슬롯 초기화
 * 슬롯 상태에 따라 UI 위젯 스위처 인덱스를 설정합니다.
 *
 * 위젯 스위처 인덱스:
 * - 0: Vacant (빈 슬롯) - "새 게임" 버튼 표시
 * - 1: EnterName (이름 입력) - 이름 입력 필드 표시
 * - 2: Taken (사용 중) - 세이브 정보 표시
 */
void UMVVM_LoadSlot::InitializeSlot()
{
	// ESaveSlotStatus 열거형 값이 위젯 스위처 인덱스와 일치
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	SetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

/**
 * 플레이어 이름 설정 (MVVM 프로퍼티)
 * @param InPlayerName - 설정할 플레이어 이름
 *
 * UE_MVVM_SET_PROPERTY_VALUE 매크로가 값 변경을 감지하고
 * 바인딩된 UI에 자동으로 알립니다.
 */
void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

/**
 * 맵 이름 설정 (MVVM 프로퍼티)
 * @param InMapName - 설정할 맵 이름
 */
void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}

/**
 * 플레이어 레벨 설정 (MVVM 프로퍼티)
 * @param InLevel - 설정할 레벨
 */
void UMVVM_LoadSlot::SetPlayerLevel(int32 InLevel)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, InLevel);
}

/**
 * 로드 슬롯 이름 설정 (MVVM 프로퍼티)
 * @param InLoadSlotName - 설정할 슬롯 이름 (파일 이름으로 사용)
 */
void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}
