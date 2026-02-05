// Copyright Druid Mechanics

/*
 * AuraAssetManager.cpp
 * Aura 프로젝트의 커스텀 에셋 매니저 구현
 *
 * 에셋 매니저는 게임 시작 시 에셋 로딩을 관리하며,
 * 이 커스텀 버전에서는 게임플레이 태그와 어빌리티 시스템 글로벌 데이터를 초기화합니다.
 *
 * 프로젝트 설정에서 이 클래스를 Asset Manager Class로 지정해야 합니다.
 */

#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

/**
 * 에셋 매니저 싱글톤 인스턴스 반환
 * @return UAuraAssetManager 참조 - GEngine에서 관리하는 에셋 매니저 인스턴스
 */
UAuraAssetManager& UAuraAssetManager::Get()
{
	// GEngine이 유효한지 확인 (초기화 순서 보장)
	check(GEngine);

	// 엔진의 에셋 매니저를 UAuraAssetManager로 캐스팅
	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

/**
 * 초기 로딩 시작 시 호출되는 함수
 * 게임 시작 시 필요한 초기화 작업을 수행합니다.
 */
void UAuraAssetManager::StartInitialLoading()
{
	// 부모 클래스의 초기 로딩 로직 실행
	Super::StartInitialLoading();

	// Aura 프로젝트에서 사용하는 모든 게임플레이 태그 등록
	FAuraGameplayTags::InitializeNativeGameplayTags();

	// Target Data 사용을 위한 필수 초기화
	// 어빌리티 시스템에서 마우스 커서 위치 등의 타겟 데이터를 사용하려면 반드시 필요
	UAbilitySystemGlobals::Get().InitGlobalData();
}
