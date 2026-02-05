// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

UINTERFACE(MinimalAPI)
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * ISaveInterface
 * 저장 인터페이스
 * 게임 저장/로드 시스템에서 액터의 상태를 저장하고 복원합니다.
 * 체크포인트, 상호작용 오브젝트, 적 등에 사용됩니다.
 */
class AURA_API ISaveInterface
{
	GENERATED_BODY()

public:

	// 저장된 트랜스폼(위치/회전)을 로드해야 하는지 여부
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool ShouldLoadTransform();

	// 액터 데이터 로드 (상태, 속성 등)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void LoadActor();
};
