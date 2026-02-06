// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class IHighlightInterface;
class UDamageTextComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class AMagicCircle;

/**
 * AAuraPlayerController
 * 플레이어 입력과 조작을 처리하는 컨트롤러 클래스
 * 마우스 커서, 이동, 어빌리티 입력 등을 담당합니다.
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	// 클라이언트에 피해량 표시 (떠오르는 데미지 숫자)
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	// 마법 원 표시 (스킬 범위 표시용)
	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	// 마법 원 숨김
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

	// === 치트 콘솔 명령어 (개발용) ===
	// 사용법: ` 키로 콘솔 열고 명령어 입력

	// 레벨 설정: SetLevel 10
	UFUNCTION(Exec)
	void SetLevel(int32 InLevel);

	// 레벨 추가: AddLevel 5
	UFUNCTION(Exec)
	void AddLevel(int32 InLevel);

	// 스킬 포인트 설정: SetSpellPoints 10
	UFUNCTION(Exec)
	void SetSpellPoints(int32 InPoints);

	// 스킬 포인트 추가: AddSpellPoints 5
	UFUNCTION(Exec)
	void AddSpellPoints(int32 InPoints);

	// 속성 포인트 설정: SetAttributePoints 10
	UFUNCTION(Exec)
	void SetAttributePoints(int32 InPoints);

	// 속성 포인트 추가: AddAttributePoints 5
	UFUNCTION(Exec)
	void AddAttributePoints(int32 InPoints);

	// XP 설정: SetXP 1000
	UFUNCTION(Exec)
	void SetXP(int32 InXP);

	// XP 추가: AddXP 500
	UFUNCTION(Exec)
	void AddXP(int32 InXP);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	// === 입력 시스템 설정 ===
	// 향상된 입력 시스템 컨텍스트
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	// 이동 입력 액션
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	// Shift 키 입력 액션
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftKeyDown = true; };  // Shift 키 눌림
	void ShiftReleased() { bShiftKeyDown = false; };  // Shift 키 해제
	bool bShiftKeyDown = false;  // Shift 키 상태

	// 캐릭터 이동 처리
	void Move(const FInputActionValue& InputActionValue);

	// === 커서 및 타게팅 시스템 ===
	void CursorTrace();  // 커서 위치 추적
	TObjectPtr<AActor> LastActor;  // 이전 프레임에 하이라이트된 액터
	TObjectPtr<AActor> ThisActor;  // 현재 프레임에 하이라이트된 액터
	FHitResult CursorHit;  // 커서 히트 결과
	static void HighlightActor(AActor* InActor);  // 액터 하이라이트 표시
	static void UnHighlightActor(AActor* InActor);  // 액터 하이라이트 제거

	// === 어빌리티 입력 처리 ===
	void AbilityInputTagPressed(FGameplayTag InputTag);  // 어빌리티 키 눌림
	void AbilityInputTagReleased(FGameplayTag InputTag);  // 어빌리티 키 해제
	void AbilityInputTagHeld(FGameplayTag InputTag);  // 어빌리티 키 유지

	// 입력 설정 데이터 에셋
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	// 어빌리티 시스템 컴포넌트 캐시
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	// 어빌리티 시스템 컴포넌트 가져오기
	UAuraAbilitySystemComponent* GetASC();

	// === UI 및 이펙트 시스템 ===
	// 데미지 텍스트 컴포넌트 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	// 마법 원 클래스
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	// 현재 생성된 마법 원 인스턴스
	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	// 마법 원 위치 업데이트
	void UpdateMagicCircleLocation();
};
