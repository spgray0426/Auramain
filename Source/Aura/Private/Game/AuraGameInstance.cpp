// Copyright Druid Mechanics

/**
 * AuraGameInstance.cpp
 *
 * 게임 인스턴스 클래스 구현 파일
 *
 * 게임 인스턴스는 게임 세션 전체에서 유지되는 전역 데이터를 관리합니다.
 * - 레벨 전환 시에도 데이터가 유지됩니다.
 * - 현재 로드된 세이브 슬롯 정보 저장
 * - 플레이어 시작 위치 태그 관리
 *
 * 주요 멤버 변수 (헤더에 정의):
 * - LoadSlotName: 현재 사용 중인 세이브 슬롯 이름
 * - LoadSlotIndex: 세이브 슬롯 인덱스 (0, 1, 2)
 * - PlayerStartTag: 맵 로드 시 플레이어 시작 위치를 결정하는 태그
 */

#include "Game/AuraGameInstance.h"

