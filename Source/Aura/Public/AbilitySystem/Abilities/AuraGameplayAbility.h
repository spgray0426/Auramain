// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * UAuraGameplayAbility
 * Aura 프로젝트의 기본 게임플레이 어빌리티 클래스
 * 모든 스킬과 능력의 베이스 클래스로 사용됩니다.
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:

	// 어빌리티를 활성화하는 입력 태그
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;

	// 현재 레벨의 어빌리티 설명 반환
	virtual FString GetDescription(int32 Level);

	// 다음 레벨의 어빌리티 설명 반환
	virtual FString GetNextLevelDescription(int32 Level);

	// 잠긴 어빌리티에 대한 설명 반환
	static FString GetLockedDescription(int32 Level);

protected:

	// 특정 레벨에서의 마나 소모량 계산
	float GetManaCost(float InLevel = 1.f) const;

	// 특정 레벨에서의 쿨다운 시간 계산
	float GetCooldown(float InLevel = 1.f) const;
};
