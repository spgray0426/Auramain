// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "AttributeInfo.generated.h"

/**
 * FAuraAttributeInfo
 * 속성 정보를 담는 구조체
 * UI에 표시할 속성의 이름과 설명을 저장합니다.
 */
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	// 속성 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	// 속성 이름 (UI에 표시)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	// 속성 설명 (UI에 표시)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	// 속성 값 (런타임에 설정)
	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};

/**
 * UAttributeInfo
 * 속성 정보 데이터 에셋
 * 게임의 모든 속성에 대한 UI 표시 정보를 관리합니다.
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// 태그로 속성 정보 찾기
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;

	// 속성 정보 배열
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;
};
