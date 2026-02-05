// Copyright Druid Mechanics

/*
 * CharacterClassInfo.cpp
 * 캐릭터 클래스 정보 데이터 에셋 구현
 *
 * 캐릭터 클래스별 기본 설정을 관리합니다:
 * - Warrior, Ranger, Elementalist 클래스별 속성
 * - 기본 어빌리티와 속성 GameplayEffect
 * - 데미지 계산 계수 커브
 */

#include "AbilitySystem/Data/CharacterClassInfo.h"

/** 캐릭터 클래스에 해당하는 기본 정보 반환 */
FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return CharacterClassInformation.FindChecked(CharacterClass);
}
