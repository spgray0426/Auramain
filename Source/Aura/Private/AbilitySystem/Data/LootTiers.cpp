// Copyright Druid Mechanics

/*
 * LootTiers.cpp
 * 루트 테이블 데이터 에셋 구현
 *
 * 적 처치 시 드롭되는 아이템을 관리합니다:
 * - 아이템별 스폰 확률
 * - 최대 스폰 개수
 * - 레벨 오버라이드 옵션
 */

#include "AbilitySystem/Data/LootTiers.h"

/**
 * 루트 아이템 목록 생성
 * 각 아이템의 스폰 확률에 따라 랜덤하게 결정
 */
TArray<FLootItem> ULootTiers::GetLootItems()
{
	TArray<FLootItem> ReturnItems;

	for (FLootItem& Item : LootItems)
	{
		// 각 아이템의 최대 스폰 개수만큼 확률 체크
		for (int32 i = 0; i < Item.MaxNumberToSpawn; ++i)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				ReturnItems.Add(NewItem);
			}
		}
	}

	return ReturnItems;
}
