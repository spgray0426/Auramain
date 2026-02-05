// Copyright Druid Mechanics

/*
 * AttributeInfo.cpp
 * 속성 정보 데이터 에셋 구현
 *
 * 캐릭터 속성의 UI 표시 정보를 관리합니다:
 * - 속성 태그로 정보 검색
 * - 속성 이름, 설명 텍스트 제공
 */

#include "AbilitySystem/Data/AttributeInfo.h"

#include "Aura/AuraLogChannels.h"

/** 태그로 속성 정보 검색 - 없으면 빈 구조체 반환 */
FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(),*GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}
