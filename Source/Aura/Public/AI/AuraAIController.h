// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBlackboardComponent;
class UBehaviorTreeComponent;

/**
 * AAuraAIController
 * 적 캐릭터를 제어하는 AI 컨트롤러
 * 비헤이비어 트리를 실행하여 적의 행동을 제어합니다.
 */
UCLASS()
class AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()
public:
	AAuraAIController();
protected:

	// 비헤이비어 트리 컴포넌트 - AI의 행동 로직을 실행
	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
