// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * UAuraUserWidget
 * Aura 사용자 위젯 베이스 클래스
 * 모든 Aura UI 위젯의 부모 클래스입니다.
 * 위젯 컨트롤러와의 연결을 관리하고 MVVM 패턴을 지원합니다.
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	// 위젯 컨트롤러 설정
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	// 위젯 컨트롤러 (데이터 소스)
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// 위젯 컨트롤러 설정 완료 이벤트 (블루프린트에서 구현)
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
