// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UPassiveNiagaraComponent;
class UDebuffNiagaraComponent;
class UNiagaraSystem;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
class UAnimMontage;

/**
 * AAuraCharacterBase
 * 모든 캐릭터의 추상 베이스 클래스
 * GAS, 전투 시스템, 애니메이션, 네트워크 복제 등 공통 기능을 제공합니다.
 * 플레이어와 AI 적 캐릭터 모두 이 클래스를 상속받습니다.
 */
UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// GAS 인터페이스
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// === Combat Interface 구현 ===
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncremenetMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	virtual FOnASCRegistered& GetOnASCRegisteredDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() override;
	virtual void SetIsBeingShocked_Implementation(bool bInShock) override;
	virtual bool IsBeingShocked_Implementation() const override;
	virtual FOnDamageSignature& GetOnDamageSignature() override;

	// === 델리게이트 ===
	FOnASCRegistered OnAscRegistered;       // ASC 등록 완료 시
	FOnDeathSignature OnDeathDelegate;      // 사망 시
	FOnDamageSignature OnDamageDelegate;    // 데미지 받을 시

	// 사망 처리 (멀티캐스트)
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);

	// === 전투 속성 ===
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontages;  // 공격 몽타주 배열

	// === 디버프 상태 (복제됨) ===
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;                // 기절 상태

	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;                 // 화상 상태

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeingShocked = false;           // 감전 상태

	// 복제 콜백
	UFUNCTION()
	virtual void OnRep_Stunned();

	UFUNCTION()
	virtual void OnRep_Burned();

	// 캐릭터 클래스 설정
	void SetCharacterClass(ECharacterClass InClass) { CharacterClass = InClass; }
protected:
	virtual void BeginPlay() override;

	// === 무기 및 소켓 ===
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;  // 무기 메시

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;                  // 무기 끝 소켓 (투사체 발사 위치)

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName LeftHandSocketName;                   // 왼손 소켓

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName RightHandSocketName;                  // 오른손 소켓

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName TailSocketName;                       // 꼬리 소켓 (적 전용)

	// === 상태 ===
	UPROPERTY(BlueprintReadOnly)
	bool bDead = false;                         // 사망 여부

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 600.f;                // 기본 이동 속도

	// === GAS 컴포넌트 ===
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	// === 기본 속성 이펙트 ===
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;      // 주요 속성 초기화

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;    // 보조 속성 초기화

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;        // 생명력 속성 초기화

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;

	void AddCharacterAbilities();

	// === 디졸브 이펙트 (사망 시) ===

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;         // 몸체 디졸브 머티리얼

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;   // 무기 디졸브 머티리얼

	// === 사망 이펙트 ===
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;                // 피 이펙트

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	USoundBase* DeathSound;                     // 사망 사운드

	// === 미니언 관리 ===
	int32 MinionCount = 0;                      // 소환한 미니언 수

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;  // 캐릭터 클래스 (전사, 레인저, 마법사 등)

	// === 디버프 이펙트 컴포넌트 ===
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> BurnDebuffComponent;    // 화상 이펙트

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDebuffNiagaraComponent> StunDebuffComponent;    // 기절 이펙트
	
private:

	// === 시작 어빌리티 ===
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;         // 시작 시 부여할 액티브 어빌리티

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;  // 시작 시 부여할 패시브 어빌리티

	// === 애니메이션 ===
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;                       // 피격 반응 몽타주

	// === 패시브 이펙트 컴포넌트 ===
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> HaloOfProtectionNiagaraComponent;  // 보호의 후광 이펙트

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> LifeSiphonNiagaraComponent;        // 생명력 흡수 이펙트

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPassiveNiagaraComponent> ManaSiphonNiagaraComponent;        // 마나 흡수 이펙트

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> EffectAttachComponent;                      // 이펙트 부착용 씬 컴포넌트
};
