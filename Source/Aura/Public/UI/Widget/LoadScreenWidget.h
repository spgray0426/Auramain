// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadScreenWidget.generated.h"

/**
 * ULoadScreenWidget
 * 로드 스크린 위젯
 * 게임 저장/로드 화면의 UI 위젯입니다.
 * MVVM 뷰모델과 연결되어 저장 슬롯 정보를 표시하고 사용자 입력을 처리합니다.
 */
UCLASS()
class AURA_API ULoadScreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	// 블루프린트에서 위젯 초기화 (블루프린트에서 구현)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BlueprintInitializeWidget();
};
