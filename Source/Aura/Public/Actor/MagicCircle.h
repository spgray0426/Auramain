// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

/**
 * AMagicCircle
 * 마법 원 액터 - 스킬 범위를 표시하는 데칼입니다.
 * 소환 스킬이나 타겟팅 스킬에서 범위를 시각적으로 보여줍니다.
 */
UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	AMagicCircle();
	virtual void Tick(float DeltaTime) override;

	// 마법 원 데칼 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;
};
