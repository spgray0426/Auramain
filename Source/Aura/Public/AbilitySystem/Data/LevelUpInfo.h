// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

/**
 * FAuraLevelUpInfo
 * 레벨업 정보를 담는 구조체
 * 각 레벨에 필요한 경험치와 보상을 정의합니다.
 */
USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	// 해당 레벨에 도달하기 위한 필요 경험치
	UPROPERTY(EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	// 레벨업 시 받는 능력치 포인트
	UPROPERTY(EditDefaultsOnly)
	int32 AttributePointAward = 1;

	// 레벨업 시 받는 주문 포인트
	UPROPERTY(EditDefaultsOnly)
	int32 SpellPointAward = 1;
};

/**
 * ULevelUpInfo
 * 레벨업 정보 데이터 에셋
 * 레벨별 경험치 요구사항과 보상을 관리합니다.
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:

	// 레벨별 정보 배열
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	// 경험치로 레벨 찾기
	int32 FindLevelForXP(int32 XP) const;
};
