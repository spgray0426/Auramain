// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;

/**
 * UAuraFireBlast
 * 화염 폭발 스킬 - 사방으로 화염구를 발사합니다.
 * 캐릭터 주변으로 여러 개의 화염구를 동시에 발사하는 광역 공격입니다.
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	// 현재 레벨의 스킬 설명 반환
	virtual FString GetDescription(int32 Level) override;

	// 다음 레벨의 스킬 설명 반환
	virtual FString GetNextLevelDescription(int32 Level) override;

	// 화염구들을 생성하고 배열로 반환
	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
protected:

	// 발사할 화염구 개수
	UPROPERTY(EditDefaultsOnly, Category = "FireBlast")
	int32 NumFireBalls = 12;

private:

	// 생성할 화염구 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireBall> FireBallClass;
};
