// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "AuraEnemySpawnPoint.generated.h"

class AAuraEnemy;

/**
 * AAuraEnemySpawnPoint
 * 적 스폰 지점
 * 레벨에 배치되어 적을 생성합니다.
 * 적 클래스, 레벨, 캐릭터 타입을 설정할 수 있습니다.
 */
UCLASS()
class AURA_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:

	// 적 생성
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	// 생성할 적 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	TSubclassOf<AAuraEnemy> EnemyClass;

	// 적 레벨
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	int32 EnemyLevel = 1;

	// 캐릭터 클래스 (전사, 레인저, 마법사 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};
