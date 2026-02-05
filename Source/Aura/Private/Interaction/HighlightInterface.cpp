// Copyright Druid Mechanics

/**
 * HighlightInterface.cpp
 *
 * 하이라이트 인터페이스 클래스 구현 파일
 *
 * 마우스 호버 시 오브젝트를 강조 표시하는 기능을 정의합니다.
 * 적, 아이템, 상호작용 가능한 오브젝트 등에서 구현됩니다.
 *
 * 주요 기능 (헤더에 선언):
 * - HighlightActor(): 액터 강조 효과 활성화 (외곽선, 발광 등)
 * - UnHighlightActor(): 액터 강조 효과 비활성화
 * - SetMoveToLocation(): 클릭 이동 시 목표 위치 설정
 *
 * 커스텀 뎁스 스텐실(Custom Depth Stencil)을 사용하여
 * 포스트 프로세싱으로 외곽선 효과를 구현합니다.
 */

#include "Interaction/HighlightInterface.h"

// 순수 가상 함수가 아닌 IHighlightInterface 함수들의 기본 구현을 여기에 추가
