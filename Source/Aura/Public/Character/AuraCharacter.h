// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;

/**
 * AAuraCharacter
 * 플레이어가 조종하는 메인 캐릭터 클래스
 * 카메라, 입력 처리, 레벨업 시스템을 담당합니다.
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
public:
	AAuraCharacter();

	// 서버에서 컨트롤러가 캐릭터를 소유할 때 호출됨
	virtual void PossessedBy(AController* NewController) override;

	// 클라이언트에서 PlayerState가 복제될 때 호출됨
	virtual void OnRep_PlayerState() override;

	/** Player Interface - 플레이어 인터페이스 */
	virtual void AddToXP_Implementation(int32 InXP) override;  // 경험치 추가
	virtual void LevelUp_Implementation() override;  // 레벨업 처리
	virtual int32 GetXP_Implementation() const override;  // 현재 경험치 반환
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;  // 경험치에 해당하는 레벨 계산
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;  // 레벨별 능력치 포인트 보상
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;  // 레벨별 주문 포인트 보상
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;  // 플레이어 레벨 추가
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;  // 능력치 포인트 추가
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;  // 주문 포인트 추가
	virtual int32 GetAttributePoints_Implementation() const override;  // 사용 가능한 능력치 포인트 반환
	virtual int32 GetSpellPoints_Implementation() const override;  // 사용 가능한 주문 포인트 반환
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;  // 마법 원 표시
	virtual void HideMagicCircle_Implementation() override;  // 마법 원 숨김
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;  // 진행상황 저장
	/** end Player Interface */

	/** Combat Interface - 전투 인터페이스 */
	virtual int32 GetPlayerLevel_Implementation() override;  // 플레이어 레벨 반환
	virtual void Die(const FVector& DeathImpulse) override;  // 사망 처리
	/** end Combat Interface */

	// 사망 후 리스폰까지의 시간 (초)
	UPROPERTY(EditDefaultsOnly)
	float DeathTime = 5.f;

	// 사망 타이머 핸들
	FTimerHandle DeathTimer;

	// 레벨업 시 재생되는 나이아가라 파티클 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	// 기절 상태 복제 처리
	virtual void OnRep_Stunned() override;

	// 화상 상태 복제 처리
	virtual void OnRep_Burned() override;

	// 저장된 진행상황 불러오기
	void LoadProgress();
private:
	// 탑다운 뷰 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	// 카메라 붐 (스프링 암) - 카메라의 위치와 거리를 조절
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;

	// 어빌리티 시스템의 액터 정보 초기화
	virtual void InitAbilityActorInfo() override;

	// 레벨업 파티클을 모든 클라이언트에 재생 (네트워크 멀티캐스트)
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
