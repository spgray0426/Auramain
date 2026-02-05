// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

/**
 * UAuraAssetManager
 * Aura 에셋 매니저
 * 게임의 에셋 로딩 및 관리를 담당합니다.
 * 게임플레이 태그를 초기화하고 필요한 에셋을 미리 로드합니다.
 */
UCLASS()
class AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:

	// 싱글톤 인스턴스 가져오기
	static UAuraAssetManager& Get();

protected:

	// 초기 로딩 시작 (게임플레이 태그 초기화)
	virtual void StartInitialLoading() override;
};
