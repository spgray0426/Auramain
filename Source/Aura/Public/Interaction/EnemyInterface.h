// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IEnemyInterface
 * 적 인터페이스
 * 적 캐릭터 전용 기능을 정의합니다 (전투 대상 관리).
 */
class AURA_API IEnemyInterface
{
	GENERATED_BODY()

public:

	// 전투 대상 설정
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCombatTarget(AActor* InCombatTarget);

	// 전투 대상 가져오기
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetCombatTarget() const;
};
