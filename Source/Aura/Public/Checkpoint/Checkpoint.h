// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Checkpoint.generated.h"

class USphereComponent;

/**
 * ACheckpoint
 * 체크포인트 액터
 * 플레이어가 도달 시 게임 진행 상황을 저장하고 리스폰 지점으로 사용됩니다.
 * 하이라이트 및 저장 인터페이스를 구현합니다.
 */
UCLASS()
class AURA_API ACheckpoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()
public:
	ACheckpoint(const FObjectInitializer& ObjectInitializer);

	// === Save Interface 구현 ===
	virtual bool ShouldLoadTransform_Implementation() override { return false; };  // 트랜스폼은 로드하지 않음
	virtual void LoadActor_Implementation() override;

	// 체크포인트 도달 여부 (저장됨)
	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bReached = false;

	// 오버랩 콜백 바인딩 여부
	UPROPERTY(EditAnywhere)
	bool bBindOverlapCallback = true;

protected:

	// 구체 오버랩 이벤트 (플레이어가 체크포인트에 도달)
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	// === Highlight Interface 구현 ===
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;

	// 이동 목적지 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;

	// 커스텀 뎁스 스텐실 값
	UPROPERTY(EditDefaultsOnly)
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	// 체크포인트 도달 시 이벤트 (블루프린트에서 구현)
	UFUNCTION(BlueprintImplementableEvent)
	void CheckpointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	// 발광 효과 처리
	UFUNCTION(BlueprintCallable)
	void HandleGlowEffects();

	// 체크포인트 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckpointMesh;

	// 트리거 구체 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
