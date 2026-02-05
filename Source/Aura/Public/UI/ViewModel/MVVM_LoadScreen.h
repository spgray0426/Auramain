// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

// 슬롯 선택 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UMVVM_LoadSlot;

/**
 * UMVVM_LoadScreen
 * 로드 스크린 뷰모델 (MVVM 패턴)
 * 게임 저장/로드 화면의 로직을 관리합니다.
 * 3개의 저장 슬롯을 제공하며, 새 게임 생성, 게임 로드, 슬롯 삭제 기능을 지원합니다.
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:

	// 로드 슬롯 초기화
	void InitializeLoadSlots();

	// 슬롯 선택 시 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;

	// 로드 슬롯 뷰모델 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotViewModelClass;

	// 인덱스로 로드 슬롯 뷰모델 가져오기
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(int32 Index) const;

	// === UI 버튼 이벤트 ===

	// 새 슬롯 버튼 클릭 (캐릭터 이름 입력 완료)
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot, const FString& EnteredName);

	// 새 게임 버튼 클릭
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	// 슬롯 선택 버튼 클릭
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(int32 Slot);

	// 삭제 버튼 클릭
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	// 플레이 버튼 클릭 (게임 시작)
	UFUNCTION(BlueprintCallable)
	void PlayButtonPressed();

	// 저장 데이터 로드
	void LoadData();

	// 로드 슬롯 개수 설정
	void SetNumLoadSlots(int32 InNumLoadSlots);

	// 로드 슬롯 개수 가져오기
	int32 GetNumLoadSlots() const { return NumLoadSlots; }
	
private:

	// 로드 슬롯 맵 (인덱스 → 뷰모델)
	UPROPERTY()
	TMap<int32, UMVVM_LoadSlot*> LoadSlots;

	// === 개별 로드 슬롯 (3개) ===
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	// 현재 선택된 슬롯
	UPROPERTY()
	UMVVM_LoadSlot* SelectedSlot;

	// 로드 슬롯 개수 (MVVM 필드 노티파이)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess="true"));
	int32 NumLoadSlots;
};
