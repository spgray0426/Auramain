// Copyright Druid Mechanics

/**
 * PlayerInterface.cpp
 *
 * 플레이어 인터페이스 클래스 구현 파일
 *
 * 플레이어 전용 기능을 정의하는 인터페이스입니다.
 * 플레이어 캐릭터가 구현하여 XP, 레벨업, 포인트 시스템 등을 지원합니다.
 *
 * 주요 기능 (헤더에 선언):
 * - AddToXP(): 경험치 추가
 * - GetXP(): 현재 경험치 반환
 * - LevelUp(): 레벨업 실행
 * - FindLevelForXP(): 경험치에 해당하는 레벨 계산
 * - GetAttributePointsReward(): 레벨업 시 속성 포인트 보상
 * - GetSpellPointsReward(): 레벨업 시 스펠 포인트 보상
 * - AddToPlayerLevel(): 플레이어 레벨 증가
 * - AddToAttributePoints(): 속성 포인트 추가
 * - AddToSpellPoints(): 스펠 포인트 추가
 * - SaveProgress(): 진행 상황 저장
 * - ShowMagicCircle(): 마법진 표시 (스펠 타겟팅)
 * - HideMagicCircle(): 마법진 숨기기
 */

#include "Interaction/PlayerInterface.h"

// 순수 가상 함수가 아닌 IPlayerInterface 함수들의 기본 구현을 여기에 추가
