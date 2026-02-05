// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_Attack.generated.h"

/**
 * UBTTask_Attack
 * 비헤이비어 트리 태스크 - 공격을 실행합니다.
 * AI가 대상을 공격하도록 하는 태스크입니다.
 */
UCLASS()
class AURA_API UBTTask_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()

	// 태스크 실행 - 공격 어빌리티를 발동합니다
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
