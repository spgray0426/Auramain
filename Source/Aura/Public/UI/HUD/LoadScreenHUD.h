// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class UMVVM_LoadScreen;
class ULoadScreenWidget;

/**
 * ALoadScreenHUD
 * 로드 스크린 HUD
 * 게임 저장/로드 화면의 UI를 관리합니다.
 * MVVM 패턴을 사용하여 저장 슬롯 선택, 캐릭터 생성, 게임 로드 기능을 제공합니다.
 */
UCLASS()
class AURA_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()
public:

	// 로드 스크린 위젯 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	// 로드 스크린 위젯 인스턴스
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	// 로드 스크린 뷰모델 클래스 (MVVM)
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadScreen> LoadScreenViewModelClass;

	// 로드 스크린 뷰모델 인스턴스
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadScreen> LoadScreenViewModel;

protected:
	virtual void BeginPlay() override;
};
