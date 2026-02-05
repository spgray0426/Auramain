// Copyright Druid Mechanics

/**
 * SaveInterface.cpp
 *
 * 세이브 인터페이스 클래스 구현 파일
 *
 * 세이브/로드 시스템과 연동되는 액터를 위한 인터페이스입니다.
 * 이 인터페이스를 구현한 액터는 게임 저장 시 상태가 보존됩니다.
 *
 * 주요 기능 (헤더에 선언):
 * - ShouldLoadTransform(): 로드 시 트랜스폼 복원 여부
 *   (체크포인트는 위치 이동 불필요, 적은 위치 복원 필요)
 * - LoadActor(): 로드 완료 후 추가 초기화 작업
 *   (적 스폰 볼륨의 경우 이미 도달했으면 파괴 등)
 *
 * 구현 예:
 * - ACheckpoint: ShouldLoadTransform = false (고정 위치)
 * - AAuraEnemySpawnVolume: LoadActor에서 도달 여부 확인
 */

#include "Interaction/SaveInterface.h"

// 순수 가상 함수가 아닌 ISaveInterface 함수들의 기본 구현을 여기에 추가
