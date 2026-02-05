// Copyright Druid Mechanics

/*
 * AuraAIController.cpp
 * AI 컨트롤러 구현
 *
 * 적 캐릭터를 제어하는 AI 컨트롤러입니다:
 * - 블랙보드 컴포넌트로 AI 상태 관리
 * - 비헤이비어 트리 컴포넌트로 행동 패턴 실행
 */

#include "AI/AuraAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

/** 생성자 - 블랙보드와 비헤이비어 트리 컴포넌트 생성 */
AAuraAIController::AAuraAIController()
{
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>("BlackboardComponent");
	check(Blackboard);
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>("BehaviorTreeComponent");
	check(BehaviorTreeComponent);
}
