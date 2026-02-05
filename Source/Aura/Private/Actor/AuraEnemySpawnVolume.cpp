// Copyright Druid Mechanics

/*
 * AuraEnemySpawnVolume.cpp
 * 적 스폰 볼륨 구현
 *
 * 플레이어가 진입하면 적을 스폰하는 트리거 영역입니다:
 * - 박스 충돌 컴포넌트로 플레이어 감지
 * - 연결된 스폰 포인트들에서 적 생성
 * - 세이브/로드 지원 (이미 도달한 경우 파괴)
 */

#include "Actor/AuraEnemySpawnVolume.h"

#include "Actor/AuraEnemySpawnPoint.h"
#include "Aura/Aura.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"

/** 생성자 - 박스 충돌 컴포넌트 설정 */
AAuraEnemySpawnVolume::AAuraEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

/** 로드 시 - 이미 도달한 볼륨이면 파괴 */
void AAuraEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

/** BeginPlay - 오버랩 이벤트 바인딩 */
void AAuraEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &AAuraEnemySpawnVolume::OnBoxOverlap);
}

/**
 * 박스 오버랩 - 플레이어 진입 시 모든 스폰 포인트에서 적 생성
 * 한 번만 작동하고 충돌 비활성화
 */
void AAuraEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	
	bReached = true;
	for (AAuraEnemySpawnPoint* Point : SpawnPoints)
	{
		if (IsValid(Point))
		{
			Point->SpawnEnemy();
		}
	}
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


