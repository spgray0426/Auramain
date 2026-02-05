// Copyright Druid Mechanics

/**
 * DamageTextComponent.cpp
 *
 * 데미지 텍스트 컴포넌트 클래스 구현 파일
 *
 * 적이 피격 시 데미지 숫자를 표시하는 위젯 컴포넌트입니다.
 * 화면 공간에서 3D 위치에 텍스트를 렌더링합니다.
 *
 * 기능:
 * - 데미지 수치 표시
 * - 치명타 여부에 따른 스타일 변경
 * - 블록 여부에 따른 스타일 변경
 * - 애니메이션 (팝업, 페이드아웃)
 *
 * 주요 함수 (헤더에 선언):
 * - SetDamageText(): 표시할 데미지 값과 속성 설정
 *
 * 블루프린트에서 애니메이션과 스타일을 구현합니다.
 */

#include "UI/Widget/DamageTextComponent.h"

