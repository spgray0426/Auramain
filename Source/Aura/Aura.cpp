// Copyright Druid Mechanics

/*
 * Aura.cpp
 * Aura 게임 모듈의 메인 진입점 구현 파일
 *
 * 이 파일은 언리얼 엔진의 게임 모듈 시스템에서 Aura 모듈을 등록하는 역할을 합니다.
 * IMPLEMENT_PRIMARY_GAME_MODULE 매크로를 통해 이 모듈이 프로젝트의 주요 게임 모듈임을 선언합니다.
 */

#include "Aura.h"
#include "Modules/ModuleManager.h"

// Aura를 프로젝트의 Primary Game Module로 등록
// FDefaultGameModuleImpl: 기본 게임 모듈 구현체 사용
// Aura: 모듈 이름 (클래스/구조체)
// "Aura": 모듈의 문자열 식별자
IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Aura, "Aura" );
