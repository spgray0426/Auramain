// Copyright Druid Mechanics

/*
 * AbilityInfo.cpp
 * 어빌리티 정보 데이터 에셋 구현
 *
 * 모든 어빌리티의 메타데이터를 관리합니다:
 * - 어빌리티 태그로 정보 검색
 * - UI 표시용 아이콘, 이름, 설명 등
 */

#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

/** 태그로 어빌리티 정보 검색 - 없으면 빈 구조체 반환 */
FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAbilityInfo();
}
