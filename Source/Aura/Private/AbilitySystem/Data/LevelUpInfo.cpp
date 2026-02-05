// Copyright Druid Mechanics

/*
 * LevelUpInfo.cpp
 * 레벨업 정보 데이터 에셋 구현
 *
 * 레벨업 시스템의 데이터를 관리합니다:
 * - 레벨별 필요 XP
 * - 레벨업 보상 (속성 포인트, 스펠 포인트)
 */

#include "AbilitySystem/Data/LevelUpInfo.h"

/**
 * XP로 레벨 계산
 * 누적 XP가 충족하는 최고 레벨을 반환
 */
int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 2 Information
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
