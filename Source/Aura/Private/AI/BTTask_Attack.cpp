// Copyright Druid Mechanics

/*
 * BTTask_Attack.cpp
 * 비헤이비어 트리 태스크 - 공격
 *
 * AI가 공격을 수행하는 태스크입니다.
 * 블루프린트에서 구체적인 공격 로직을 구현합니다.
 */

#include "AI/BTTask_Attack.h"

/** 태스크 실행 - 부모 클래스의 공격 로직 호출 */
EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
