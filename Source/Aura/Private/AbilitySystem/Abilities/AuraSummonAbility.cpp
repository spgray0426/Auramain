// Copyright Druid Mechanics

/*
 * AuraSummonAbility.cpp
 * 소환 어빌리티 구현
 *
 * 미니언(소환수)을 소환하는 어빌리티입니다.
 * - 시전자 전방에 부채꼴 형태로 스폰 위치 계산
 * - 지형에 맞춰 스폰 위치 조정 (라인 트레이스)
 * - 랜덤 미니언 클래스 선택
 */

#include "AbilitySystem/Abilities/AuraSummonAbility.h"

/**
 * 소환 위치 계산
 * 시전자 전방에 NumMinions 개의 스폰 위치를 계산합니다.
 * 지면 높이에 맞춰 위치를 조정합니다.
 */
TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i = 0; i < NumMinions; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		FVector ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenSpawnLocation);
	}
	
	return SpawnLocations;
}

/** 랜덤 미니언 클래스 반환 */
TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClasses.Num() - 1);
	return MinionClasses[Selection];
}
