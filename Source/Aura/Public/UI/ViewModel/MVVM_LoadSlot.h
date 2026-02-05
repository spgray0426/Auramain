// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVM_LoadSlot.generated.h"

// 위젯 스위처 인덱스 설정 델리게이트 (슬롯 상태 UI 변경)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
// 슬롯 선택 버튼 활성화 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 * UMVVM_LoadSlot
 * 로드 슬롯 뷰모델 (MVVM 패턴)
 * 개별 저장 슬롯의 데이터와 상태를 관리합니다.
 * 슬롯 상태: 비어있음, 이름 입력 필요, 저장 데이터 있음
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	// 위젯 스위처 인덱스 설정 델리게이트 (UI 상태 변경)
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;

	// 슬롯 선택 버튼 활성화 델리게이트
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;

	// 슬롯 초기화
	void InitializeSlot();

	// 슬롯 인덱스 (0, 1, 2)
	UPROPERTY()
	int32 SlotIndex;

	// 슬롯 상태 (비어있음, 이름 입력, 저장됨)
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	// 플레이어 시작 지점 태그
	UPROPERTY()
	FName PlayerStartTag;

	// 맵 에셋 이름
	UPROPERTY()
	FString MapAssetName;

	// === MVVM 필드 노티파이 Setter/Getter ===

	void SetPlayerName(FString InPlayerName);
	void SetMapName(FString InMapName);
	void SetPlayerLevel(int32 InLevel);
	void SetLoadSlotName(FString InLoadSlotName);

	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
	FString GetLoadSlotName() const { return LoadSlotName; }

private:

	// 플레이어 이름 (MVVM 바인딩)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"));
	FString PlayerName;

	// 맵 이름 (MVVM 바인딩)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"));
	FString MapName;

	// 플레이어 레벨 (MVVM 바인딩)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"));
	int32 PlayerLevel;

	// 로드 슬롯 이름 (MVVM 바인딩)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"));
	FString LoadSlotName;
};
