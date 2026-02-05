// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * UElectrocute
 * 전기 충격 스킬 - 빔 형태로 적을 공격합니다.
 * 주 대상에서 시작하여 주변 적들에게 연쇄적으로 전기가 퍼집니다.
 */
UCLASS()
class AURA_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
public:
	// 현재 레벨의 스킬 설명 반환
	virtual FString GetDescription(int32 Level) override;

	// 다음 레벨의 스킬 설명 반환
	virtual FString GetNextLevelDescription(int32 Level) override;
};
