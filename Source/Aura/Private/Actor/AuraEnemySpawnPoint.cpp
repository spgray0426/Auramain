// Copyright Druid Mechanics

/*
 * AuraEnemySpawnPoint.cpp
 * 적 스폰 포인트 구현
 *
 * 적 캐릭터가 스폰될 위치를 지정합니다:
 * - 에디터에서 배치하여 스폰 위치 설정
 * - 스폰될 적의 클래스, 레벨, 캐릭터 클래스 지정
 */

#include "Actor/AuraEnemySpawnPoint.h"

#include "Character/AuraEnemy.h"

/**
 * 적 스폰
 * 지연 스폰으로 레벨과 클래스 설정 후 스폰 완료
 */
void AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
