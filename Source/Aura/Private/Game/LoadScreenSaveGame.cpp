// Copyright Druid Mechanics

/**
 * LoadScreenSaveGame.cpp
 *
 * 세이브 게임 데이터 클래스 구현 파일
 *
 * 게임 저장 시 사용되는 데이터 컨테이너입니다.
 * - 플레이어 기본 정보 (이름, 레벨)
 * - 현재 맵 정보
 * - 각 맵별 저장된 액터 상태
 * - 플레이어 어빌리티 및 속성 데이터
 *
 * USaveGame을 상속받아 언리얼의 세이브 시스템과 호환됩니다.
 */

#include "Game/LoadScreenSaveGame.h"

/**
 * 맵 이름으로 저장된 맵 데이터 조회
 * @param InMapName - 찾을 맵 에셋 이름
 * @return 해당 맵의 저장 데이터 (없으면 빈 구조체)
 */
FSavedMap ULoadScreenSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return Map;
		}
	}
	return FSavedMap();
}

/**
 * 맵 존재 여부 확인
 * @param InMapName - 확인할 맵 에셋 이름
 * @return 해당 맵의 저장 데이터 존재 여부
 */
bool ULoadScreenSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap& Map : SavedMaps)
	{
		if (Map.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}
