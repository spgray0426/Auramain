// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UWidgetComponent;
class UBehaviorTree;
class AAuraAIController;

/**
 * AAuraEnemy
 * 적(Enemy) 캐릭터 클래스
 * AI 컨트롤러, 체력바 UI, 하이라이트 기능 등을 담당합니다.
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface, public IHighlightInterface
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	// AI 컨트롤러가 적을 소유할 때 호출
	virtual void PossessedBy(AController* NewController) override;

	/** Highlight Interface - 하이라이트 인터페이스 */
	virtual void HighlightActor_Implementation() override;  // 액터 하이라이트 표시
	virtual void UnHighlightActor_Implementation() override;  // 액터 하이라이트 제거
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;  // 이동 목표 위치 설정
	/** end Highlight Interface */

	/** Combat Interface - 전투 인터페이스 */
	virtual int32 GetPlayerLevel_Implementation() override;  // 레벨 반환
	virtual void Die(const FVector& DeathImpulse) override;  // 사망 처리
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;  // 전투 대상 설정
	virtual AActor* GetCombatTarget_Implementation() const override;  // 전투 대상 반환
	/** end Combat Interface */

	// 현재 전투 중인 대상
	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	// 체력 변경 시 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	// 최대 체력 변경 시 호출되는 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	// 피격 반응 태그 변경 처리
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	// 피격 반응 중인지 여부
	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	// 사망 후 소멸까지의 시간 (초)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	// 레벨 설정
	void SetLevel(int32 InLevel) { Level = InLevel; }
protected:
	virtual void BeginPlay() override;

	// 어빌리티 시스템의 액터 정보 초기화
	virtual void InitAbilityActorInfo() override;

	// 기본 속성값 초기화
	virtual void InitializeDefaultAttributes() const override;

	// 기절 태그 변경 처리
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	// 적의 레벨 (높을수록 강함)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	// 체력바 UI 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	// AI 행동 트리
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	// AI 컨트롤러 참조
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;

	// 사망 시 전리품 생성 (블루프린트에서 구현)
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
};
