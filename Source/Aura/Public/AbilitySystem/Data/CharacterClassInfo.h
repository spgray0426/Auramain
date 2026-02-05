// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScalableFloat.h"
#include "CharacterClassInfo.generated.h"


class UGameplayEffect;
class UGameplayAbility;

/**
 * ECharacterClass
 * 캐릭터 클래스 열거형
 */
UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,  // 원소술사 - 마법 중심
	Warrior,       // 전사 - 근접 전투
	Ranger         // 레인저 - 원거리 공격
};

/**
 * FCharacterClassDefaultInfo
 * 캐릭터 클래스별 기본 정보를 담는 구조체
 */
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	// 주요 속성 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	// 시작 어빌리티 목록
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	// 처치 시 얻는 경험치 보상 (레벨에 따라 스케일됨)
	UPROPERTY(EditDefaultsOnly, Category = "Class Defaults")
	FScalableFloat XPReward = FScalableFloat();
};

/**
 * UCharacterClassInfo
 * 캐릭터 클래스 정보 데이터 에셋
 * 각 캐릭터 클래스의 기본 속성과 어빌리티를 정의합니다.
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	// 캐릭터 클래스별 정보 맵
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;

	// 호출자가 설정하는 주요 속성 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes_SetByCaller;

	// 2차 속성 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	// 무한 지속 2차 속성 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes_Infinite;

	// 생명력 관련 속성 이펙트
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	// 모든 클래스 공통 어빌리티
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	// 데미지 계산 계수 커브 테이블
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Defaults|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficients;

	// 특정 클래스의 기본 정보 가져오기
	FCharacterClassDefaultInfo GetClassDefaultInfo(ECharacterClass CharacterClass);
};
