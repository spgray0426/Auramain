// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * UDamageTextComponent
 * 데미지 텍스트 컴포넌트
 * 데미지를 받을 때 캐릭터 위에 표시되는 플로팅 데미지 숫자 위젯입니다.
 * 일반 데미지, 막기, 치명타에 따라 다른 색상과 크기로 표시됩니다.
 */
UCLASS()
class AURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	// 데미지 텍스트 설정 (블루프린트에서 구현)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bBlockedHit, bool bCriticalHit);
};
