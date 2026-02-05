// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

/**
 * UAuraGameInstance
 * Aura 게임 인스턴스
 * 게임 세션 전체에 걸쳐 지속되는 데이터를 관리합니다.
 * 맵 전환 시에도 유지되는 플레이어 시작 지점과 저장 슬롯 정보를 보관합니다.
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	// 플레이어 시작 지점 태그 (맵 전환 시 사용)
	UPROPERTY()
	FName PlayerStartTag = FName();

	// 로드할 저장 슬롯 이름
	UPROPERTY()
	FString LoadSlotName = FString();

	// 로드할 저장 슬롯 인덱스 (0, 1, 2)
	UPROPERTY()
	int32 LoadSlotIndex = 0;
};
