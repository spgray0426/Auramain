// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AuraInputConfig.generated.h"

/**
 * FAuraInputAction
 * 입력 액션과 게임플레이 태그를 연결하는 구조체
 */
USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_BODY()

	// 향상된 입력 시스템의 입력 액션
	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	// 입력 태그 (어빌리티와 매칭됨)
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag = FGameplayTag();
};

/**
 * UAuraInputConfig
 * 입력 설정 데이터 에셋
 * 입력 액션과 어빌리티 태그의 매핑을 관리합니다.
 */
UCLASS()
class AURA_API UAuraInputConfig : public UDataAsset
{
	GENERATED_BODY()
public:

	// 태그로 어빌리티 입력 액션 찾기
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	// 어빌리티 입력 액션 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraInputAction> AbilityInputActions;
};
