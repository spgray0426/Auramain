// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

/**
 * UBTService_FindNearestPlayer
 * 비헤이비어 트리 서비스 - 가장 가까운 플레이어를 찾습니다.
 * 매 틱마다 실행되어 블랙보드에 가장 가까운 플레이어와 거리를 저장합니다.
 */
UCLASS()
class AURA_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()
protected:

	// 서비스 틱 - 매 프레임마다 가장 가까운 플레이어를 찾습니다
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 추적할 대상을 저장할 블랙보드 키
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	// 대상까지의 거리를 저장할 블랙보드 키
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;
};
