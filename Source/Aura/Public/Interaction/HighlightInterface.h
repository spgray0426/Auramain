// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighlightInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType)
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IHighlightInterface
 * 하이라이트 인터페이스
 * 마우스 커서가 액터 위에 있을 때 시각적 하이라이트 효과를 제공합니다.
 * 적, 상호작용 가능한 오브젝트 등에 사용됩니다.
 */
class AURA_API IHighlightInterface
{
	GENERATED_BODY()

public:

	// 액터 하이라이트 시작
	UFUNCTION(BlueprintNativeEvent)
	void HighlightActor();

	// 액터 하이라이트 해제
	UFUNCTION(BlueprintNativeEvent)
	void UnHighlightActor();

	// 이동 목적지 설정 (클릭한 위치로 이동)
	UFUNCTION(BlueprintNativeEvent)
	void SetMoveToLocation(FVector& OutDestination);
};
