// Copyright Druid Mechanics

/*
 * MagicCircle.cpp
 * 매직 서클 구현
 *
 * 스킬 시전 위치를 표시하는 데칼 액터입니다:
 * - 커서 위치를 따라다니며 스킬 범위/위치 표시
 * - 스킬별 다른 머티리얼 적용 가능
 */

#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"

/** 생성자 - 매직 서클 데칼 컴포넌트 생성 */
AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

