// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "AuraEnemySpawnVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;

/**
 * AAuraEnemySpawnVolume
 * 적 스폰 볼륨
 * 플레이어가 영역에 진입하면 여러 적 스폰 지점에서 적들을 동시에 생성합니다.
 * 한 번 도달하면 상태가 저장되어 재진입 시 적이 다시 생성되지 않습니다.
 */
UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:
	AAuraEnemySpawnVolume();

	// === Save Interface 구현 ===
	virtual void LoadActor_Implementation() override;

	// 도달 여부 (저장됨)
	UPROPERTY(BlueprintReadOnly, SaveGame)
	bool bReached = false;

protected:
	virtual void BeginPlay() override;

	// 박스 오버랩 이벤트 (플레이어 진입 시 적 생성)
	UFUNCTION()
	virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 스폰 지점 배열
	UPROPERTY(EditAnywhere)
	TArray<AAuraEnemySpawnPoint*> SpawnPoints;

private:

	// 트리거 박스 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> Box;
};
