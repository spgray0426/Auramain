// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IPlayerInterface
 * 플레이어 인터페이스
 * 플레이어 캐릭터 전용 기능을 정의합니다 (경험치, 레벨업 등).
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:

	// 경험치로 레벨 찾기
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	// 현재 경험치 가져오기
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	// 레벨별 능력치 포인트 보상
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	// 레벨별 주문 포인트 보상
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;

	// 경험치 추가
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	// 플레이어 레벨 추가
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	// 능력치 포인트 추가
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	// 능력치 포인트 가져오기
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	// 주문 포인트 추가
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);

	// 주문 포인트 가져오기
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	// 레벨업 처리
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	// 마법 원 표시
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	// 마법 원 숨김
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideMagicCircle();

	// 진행 상황 저장
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& CheckpointTag);
};
