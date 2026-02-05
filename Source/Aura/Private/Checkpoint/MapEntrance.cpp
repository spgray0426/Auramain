// Copyright Druid Mechanics

/*
 * MapEntrance.cpp
 * 맵 입구 구현
 *
 * 다른 맵으로 이동하는 포탈 역할을 합니다:
 * - Checkpoint를 상속하여 하이라이트 기능 제공
 * - 플레이어 도달 시 목표 맵으로 레벨 전환
 * - 진행 상황 저장 후 맵 이동
 */

#include "Checkpoint/MapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

/** 생성자 - 스피어를 MoveToComponent에 부착 */
AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

/** 하이라이트 - 항상 하이라이트 가능 (체크포인트와 달리 도달 여부 무관) */
void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

/** 로드 시 - 맵 입구는 특별한 처리 없음 */
void AMapEntrance::LoadActor_Implementation()
{
	// 맵 입구는 로드 시 특별한 처리가 필요 없음
}

/**
 * 스피어 오버랩 - 플레이어 도달 시 목표 맵으로 이동
 * 월드 상태 저장 후 새 맵 로드
 */
void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		
		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
