// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * AMapEntrance
 * 맵 입구 액터
 * 다른 맵으로 이동할 수 있는 포탈/입구입니다.
 * 플레이어가 오버랩하면 목적지 맵으로 전환합니다.
 * 체크포인트 기능도 포함하여 진행 상황을 저장합니다.
 */
UCLASS()
class AURA_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()
public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	// === Highlight Interface 구현 ===
	virtual void HighlightActor_Implementation() override;

	// === Save Interface 구현 ===
	virtual void LoadActor_Implementation() override;

	// 목적지 맵 (소프트 레퍼런스)
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	// 목적지 플레이어 시작 지점 태그
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;

protected:

	// 구체 오버랩 이벤트 (맵 전환 트리거)
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;

};
