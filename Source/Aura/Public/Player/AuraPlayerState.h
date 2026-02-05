// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

// 플레이어 스탯 변경 델리게이트
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)
// 레벨 변경 델리게이트
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/)

/**
 * AAuraPlayerState
 * 플레이어 상태 클래스
 * 레벨, 경험치, 능력치 포인트 등 플레이어의 진행 상황을 관리합니다.
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AAuraPlayerState();

	// 네트워크 복제 속성 등록
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 어빌리티 시스템 컴포넌트 가져오기
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// 속성 세트 가져오기
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// 레벨업 정보 데이터 에셋
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	// 델리게이트들
	FOnPlayerStatChanged OnXPChangedDelegate;            // 경험치 변경 시
	FOnLevelChanged OnLevelChangedDelegate;               // 레벨 변경 시
	FOnPlayerStatChanged OnAttributePointsChangedDelegate; // 능력치 포인트 변경 시
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;    // 주문 포인트 변경 시

	// Getter 함수들
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	// 추가 함수들
	void AddToXP(int32 InXP);                    // 경험치 추가
	void AddToLevel(int32 InLevel);              // 레벨 추가
	void AddToAttributePoints(int32 InPoints);   // 능력치 포인트 추가
	void AddToSpellPoints(int32 InPoints);       // 주문 포인트 추가

	// 설정 함수들
	void SetXP(int32 InXP);                      // 경험치 설정
	void SetLevel(int32 InLevel);                // 레벨 설정
	void SetAttributePoints(int32 InPoints);     // 능력치 포인트 설정
	void SetSpellPoints(int32 InPoints);         // 주문 포인트 설정
	
protected:

	// 어빌리티 시스템 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// 속성 세트
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	// 플레이어 레벨 (복제됨)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	// 경험치 (복제됨)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	// 능력치 포인트 (복제됨)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 0;

	// 주문 포인트 (복제됨)
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints = 0;

	// 레벨 복제 콜백
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	// 경험치 복제 콜백
	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	// 능력치 포인트 복제 콜백
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	// 주문 포인트 복제 콜백
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
