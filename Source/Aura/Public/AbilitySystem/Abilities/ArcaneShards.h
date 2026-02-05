// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 * UArcaneShards
 * 비전 파편 스킬 - 여러 개의 마법 파편을 발사합니다.
 * 레벨에 따라 발사되는 파편의 개수가 증가합니다.
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	// 현재 레벨의 스킬 설명 반환
	virtual FString GetDescription(int32 Level) override;

	// 다음 레벨의 스킬 설명 반환
	virtual FString GetNextLevelDescription(int32 Level) override;

	// 최대 발사 가능한 파편 개수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumShards = 11;
};
