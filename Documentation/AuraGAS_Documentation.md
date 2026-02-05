# Aura GameplayAbilitySystem 문서

## 목차
1. [프로젝트 개요](#1-프로젝트-개요)
2. [시스템 아키텍처](#2-시스템-아키텍처)
3. [핵심 클래스](#3-핵심-클래스)
4. [API 레퍼런스](#4-api-레퍼런스)
5. [튜토리얼](#5-튜토리얼)
6. [데이터 에셋](#6-데이터-에셋)
7. [네트워크 복제](#7-네트워크-복제)

---

## 1. 프로젝트 개요

### 1.1 소개
Aura는 언리얼 엔진 5의 GameplayAbilitySystem(GAS)을 활용한 탑다운 ARPG 프로젝트입니다.
디아블로 스타일의 클릭 이동, 다양한 마법 스킬, RPG 성장 시스템을 구현합니다.

### 1.2 주요 기능
- **어빌리티 시스템**: 투사체, 빔, 범위 스킬, 소환 등 다양한 스킬 타입
- **속성 시스템**: Primary/Secondary/Vital 속성과 저항력
- **레벨업 시스템**: XP 획득, 레벨업, 포인트 분배
- **AI 시스템**: 비헤이비어 트리 기반 적 AI
- **세이브/로드**: 진행 상황 저장 및 복원
- **UI 시스템**: MVVM 패턴 기반 위젯

### 1.3 폴더 구조
```
Source/Aura/
├── Public/                 # 헤더 파일
│   ├── AbilitySystem/      # GAS 관련
│   │   ├── Abilities/      # 게임플레이 어빌리티
│   │   ├── Data/           # 데이터 에셋
│   │   ├── ModMagCalc/     # 수치 계산
│   │   └── ExecCalc/       # 데미지 계산
│   ├── Actor/              # 투사체, 이펙트 액터
│   ├── AI/                 # AI 컨트롤러, BT 노드
│   ├── Character/          # 캐릭터 클래스
│   ├── Player/             # 플레이어 컨트롤러/스테이트
│   ├── UI/                 # HUD, 위젯, 뷰모델
│   └── Interaction/        # 인터페이스
└── Private/                # 구현 파일
```

---

## 2. 시스템 아키텍처

### 2.1 GAS 핵심 구조

```
┌─────────────────────────────────────────────────────────────┐
│                    AbilitySystemComponent                    │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
│  │ Abilities   │  │ Attributes  │  │ GameplayEffects     │  │
│  │ (스킬)      │  │ (속성)       │  │ (버프/디버프)        │  │
│  └─────────────┘  └─────────────┘  └─────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      GameplayTags                            │
│         (모든 시스템을 연결하는 태그 기반 식별자)              │
└─────────────────────────────────────────────────────────────┘
```

### 2.2 캐릭터 계층 구조

```
AAuraCharacterBase (추상 기본 클래스)
├── AAuraCharacter (플레이어)
│   └── ASC는 PlayerState에서 관리
└── AAuraEnemy (적)
    └── ASC는 자체적으로 관리
```

### 2.3 데이터 흐름

```
[Input] → [PlayerController] → [ASC] → [Ability] → [GameplayEffect] → [AttributeSet]
                                  │
                                  ▼
                           [GameplayCue]
                           (시각/청각 피드백)
```

### 2.4 UI 아키텍처 (MVVM)

```
┌─────────────┐    ┌──────────────────┐    ┌─────────────┐
│   Model     │ ←→ │ WidgetController │ ←→ │    View     │
│ (ASC/Data)  │    │   (ViewModel)    │    │  (Widget)   │
└─────────────┘    └──────────────────┘    └─────────────┘
```

---

## 3. 핵심 클래스

### 3.1 AbilitySystemComponent

#### UAuraAbilitySystemComponent
어빌리티 시스템의 핵심 컴포넌트입니다.

**주요 기능:**
- 어빌리티 부여 및 활성화
- 입력 태그 기반 어빌리티 트리거
- 스킬 포인트를 통한 업그레이드
- 어빌리티 슬롯 장착

**위치:** `Source/Aura/Public/AbilitySystem/AuraAbilitySystemComponent.h`

### 3.2 AttributeSet

#### UAuraAttributeSet
캐릭터의 모든 속성을 정의합니다.

**Primary 속성:**
| 속성 | 설명 | 영향 |
|------|------|------|
| Strength | 힘 | 물리 데미지 |
| Intelligence | 지능 | 마법 데미지, 최대 마나 |
| Resilience | 탄력성 | 방어력, 방어 관통 |
| Vigor | 활력 | 최대 체력 |

**Secondary 속성:**
| 속성 | 설명 |
|------|------|
| MaxHealth | 최대 체력 |
| MaxMana | 최대 마나 |
| Armor | 방어력 |
| ArmorPenetration | 방어 관통 |
| BlockChance | 블록 확률 |
| CriticalHitChance | 크리티컬 확률 |
| CriticalHitDamage | 크리티컬 데미지 |
| CriticalHitResistance | 크리티컬 저항 |
| HealthRegeneration | 체력 재생 |
| ManaRegeneration | 마나 재생 |

**저항력:**
- FireResistance, LightningResistance, ArcaneResistance, PhysicalResistance

**위치:** `Source/Aura/Public/AbilitySystem/AuraAttributeSet.h`

### 3.3 GameplayAbility

#### UAuraGameplayAbility
모든 어빌리티의 기본 클래스입니다.

**주요 파생 클래스:**

| 클래스 | 설명 |
|--------|------|
| UAuraDamageGameplayAbility | 데미지를 주는 어빌리티의 기본 클래스 |
| UAuraProjectileSpell | 투사체 발사 스킬 |
| UAuraBeamSpell | 빔 형태 스킬 (일렉트로큐트) |
| UAuraFireBlast | 방사형 파이어볼 스킬 |
| UArcaneShards | 다중 타겟 비전 파편 |
| UAuraMeleeAttack | 근접 공격 |
| UAuraPassiveAbility | 패시브 스킬 |
| UAuraSummonAbility | 소환 스킬 |

### 3.4 Character

#### AAuraCharacterBase
모든 캐릭터의 기본 클래스입니다.

**주요 컴포넌트:**
- Weapon (USkeletalMeshComponent): 무기 메시
- BurnDebuffComponent: 화상 디버프 이펙트
- StunDebuffComponent: 기절 디버프 이펙트
- 패시브 나이아가라 컴포넌트들

#### AAuraCharacter (플레이어)
**특징:**
- PlayerState의 ASC 사용
- 카메라 및 스프링 암
- 레벨업 나이아가라 이펙트
- 세이브/로드 기능

#### AAuraEnemy (적)
**특징:**
- 자체 ASC 보유
- 체력바 위젯
- AI 비헤이비어 트리
- 루트 드롭

### 3.5 데미지 시스템

#### UExecCalc_Damage
데미지 계산의 핵심 클래스입니다.

**계산 순서:**
1. 디버프 판정 (화상, 기절 등)
2. 속성 저항 적용
3. 방사형 데미지 처리 (해당시)
4. 블록 판정 (50% 감소)
5. 방어력/방어 관통 계산
6. 크리티컬 판정 (2배 + 보너스)

**데미지 공식:**
```
최종 데미지 = 기본 데미지
    × (1 - 저항력/100)
    × (블록 ? 0.5 : 1)
    × (1 - 유효 방어력 × 계수/100)
    × (크리티컬 ? 2 + 크리티컬 보너스 : 1)
```

---

## 4. API 레퍼런스

### 4.1 UAuraAbilitySystemComponent

#### 어빌리티 관리

```cpp
// 캐릭터 어빌리티 부여
void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

// 패시브 어빌리티 부여
void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

// 세이브 데이터에서 어빌리티 복원
void AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveData);
```

#### 입력 처리

```cpp
// 입력 태그로 어빌리티 트리거
void AbilityInputTagPressed(const FGameplayTag& InputTag);
void AbilityInputTagReleased(const FGameplayTag& InputTag);
void AbilityInputTagHeld(const FGameplayTag& InputTag);
```

#### 어빌리티 업그레이드

```cpp
// 스펠 포인트로 어빌리티 업그레이드
void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

// 어빌리티 슬롯 장착
void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);
```

#### 상태 조회

```cpp
// 어빌리티 태그로 상태 조회
FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);

// 어빌리티 태그로 슬롯 조회
FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);

// 입력 태그로 어빌리티 Spec 조회
FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
```

### 4.2 UAuraAttributeSet

#### 속성 접근

```cpp
// 정적 함수로 속성 접근
static FGameplayAttribute GetStrengthAttribute();
static FGameplayAttribute GetIntelligenceAttribute();
static FGameplayAttribute GetResilienceAttribute();
static FGameplayAttribute GetVigorAttribute();
// ... 모든 속성에 대해 동일한 패턴

// 현재 값 가져오기
float GetHealth() const;
float GetMaxHealth() const;
float GetMana() const;
float GetMaxMana() const;
// ... 모든 속성에 대해 동일한 패턴
```

### 4.3 UAuraAbilitySystemLibrary

#### 유틸리티 함수

```cpp
// 위젯 컨트롤러 파라미터 생성
static FWidgetControllerParams MakeWidgetControllerParams(APlayerController* PC);

// 오버레이 위젯 컨트롤러 가져오기
static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

// 속성 메뉴 위젯 컨트롤러 가져오기
static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

// 스펠 메뉴 위젯 컨트롤러 가져오기
static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
```

#### 캐릭터 초기화

```cpp
// 캐릭터 클래스별 기본 속성 초기화
static void InitializeDefaultAttributes(const UObject* WorldContextObject,
    ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

// 시작 어빌리티 부여
static void GiveStartupAbilities(const UObject* WorldContextObject,
    UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
```

#### 데미지 처리

```cpp
// 데미지 이펙트 적용
static void ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

// 방사형 데미지 적용
static void ApplyRadialDamageWithFalloff(const UObject* WorldContextObject, ...);

// 범위 내 플레이어 가져오기
static void GetLivePlayersWithinRadius(const UObject* WorldContextObject,
    TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore,
    float Radius, const FVector& SphereOrigin);
```

#### GameplayEffect Context 접근자

```cpp
// Setter
static void SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
static void SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
static void SetIsSuccessfulDebuff(FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff);
static void SetDebuffDamage(FGameplayEffectContextHandle& EffectContextHandle, float InDamage);
static void SetDebuffDuration(FGameplayEffectContextHandle& EffectContextHandle, float InDuration);
static void SetDebuffFrequency(FGameplayEffectContextHandle& EffectContextHandle, float InFrequency);
static void SetDamageType(FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);
static void SetDeathImpulse(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse);
static void SetKnockbackForce(FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce);

// Getter
static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);
static FVector GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle);
static FVector GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle);
```

### 4.4 ICombatInterface

```cpp
// 레벨 가져오기
virtual int32 GetPlayerLevel() = 0;

// 전투 소켓 위치 (무기, 손 등)
virtual FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) = 0;

// 공격 몽타주 가져오기
virtual TArray<FTaggedMontage> GetAttackMontages() = 0;

// 사망 여부
virtual bool IsDead() const = 0;

// 아바타 액터 가져오기
virtual AActor* GetAvatar() = 0;

// 미니언 카운트
virtual int32 GetMinionCount() = 0;
virtual void IncremenetMinionCount(int32 Amount) = 0;

// 캐릭터 클래스
virtual ECharacterClass GetCharacterClass() = 0;

// 무기 컴포넌트
virtual USkeletalMeshComponent* GetWeapon() = 0;

// 감전 상태
virtual void SetIsBeingShocked(bool bInShock) = 0;
virtual bool IsBeingShocked() const = 0;
```

---

## 5. 튜토리얼

### 5.1 새로운 어빌리티 추가하기

#### 단계 1: 어빌리티 클래스 생성

```cpp
// MyNewAbility.h
#pragma once

#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "MyNewAbility.generated.h"

UCLASS()
class AURA_API UMyNewAbility : public UAuraDamageGameplayAbility
{
    GENERATED_BODY()

public:
    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle,
        const FGameplayAbilityActorInfo* ActorInfo,
        const FGameplayAbilityActivationInfo ActivationInfo,
        const FGameplayEventData* TriggerEventData) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Ability")
    float AbilityDamage = 50.f;
};
```

```cpp
// MyNewAbility.cpp
#include "AbilitySystem/Abilities/MyNewAbility.h"

void UMyNewAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    // 어빌리티 로직 구현
    // ...

    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
```

#### 단계 2: GameplayTag 추가

`AuraGameplayTags.cpp`에서:
```cpp
GameplayTags.Abilities_MyNewAbility = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Abilities.MyNewAbility"),
    FString("My New Ability")
);
```

#### 단계 3: AbilityInfo 데이터 에셋에 등록

에디터에서 `DA_AbilityInfo` 데이터 에셋을 열고:
- AbilityTag: `Abilities.MyNewAbility`
- Ability: `MyNewAbility` 클래스
- AbilityType: 원하는 타입 선택
- Icon, Background, 설명 등 설정

#### 단계 4: 입력 바인딩 (선택사항)

`DA_InputConfig`에서:
- InputTag: 원하는 입력 태그
- AbilityTag: `Abilities.MyNewAbility`

### 5.2 새로운 속성 추가하기

#### 단계 1: AttributeSet에 속성 추가

```cpp
// AuraAttributeSet.h에 추가
UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MyNewAttribute, Category = "Attributes")
FGameplayAttributeData MyNewAttribute;
ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MyNewAttribute);

UFUNCTION()
void OnRep_MyNewAttribute(const FGameplayAttributeData& OldMyNewAttribute) const;
```

```cpp
// AuraAttributeSet.cpp에 추가
void UAuraAttributeSet::OnRep_MyNewAttribute(const FGameplayAttributeData& OldMyNewAttribute) const
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MyNewAttribute, OldMyNewAttribute);
}

// GetLifetimeReplicatedProps에 추가
DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MyNewAttribute, COND_None, REPNOTIFY_Always);
```

#### 단계 2: GameplayTag 추가

```cpp
GameplayTags.Attributes_MyNewAttribute = UGameplayTagsManager::Get().AddNativeGameplayTag(
    FName("Attributes.MyNewAttribute"),
    FString("My New Attribute")
);
```

#### 단계 3: AttributeInfo 데이터 에셋에 등록

에디터에서 해당 속성의 표시 이름과 설명 추가

### 5.3 새로운 적 캐릭터 추가하기

#### 단계 1: 블루프린트 생성

1. `BP_EnemyBase`를 부모로 새 블루프린트 생성
2. 메시와 애니메이션 설정
3. CharacterClass 설정 (Warrior, Ranger, Elementalist)
4. Level 설정

#### 단계 2: 비헤이비어 트리 설정

1. BehaviorTree 에셋 지정 또는 새로 생성
2. Blackboard에 필요한 키 추가:
   - `TargetToFollow` (Object)
   - `DistanceToTarget` (Float)
   - `HitReacting` (Bool)
   - `Dead` (Bool)
   - `RangedAttacker` (Bool)

#### 단계 3: 공격 몽타주 설정

`AttackMontages` 배열에 FTaggedMontage 추가:
- Montage: 공격 애니메이션
- MontageTag: 공격 식별 태그
- SocketTag: 데미지 발생 소켓

### 5.4 세이브/로드 시스템 사용하기

#### 저장

```cpp
// 체크포인트에서 자동 저장
// 또는 수동 호출:
if (AAuraCharacter* AuraCharacter = Cast<AAuraCharacter>(GetPawn()))
{
    IPlayerInterface::Execute_SaveProgress(AuraCharacter, CheckpointTag);
}
```

#### 저장되는 데이터

- 플레이어 레벨, XP
- 속성 포인트, 스펠 포인트
- Primary 속성값
- 모든 어빌리티 상태 (레벨, 슬롯, 잠금해제 상태)
- 현재 맵과 체크포인트 위치

---

## 6. 데이터 에셋

### 6.1 DA_AbilityInfo
모든 어빌리티의 메타데이터를 저장합니다.

**구조:**
```cpp
USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
    FGameplayTag AbilityTag;           // 어빌리티 식별 태그
    FGameplayTag InputTag;             // 입력 태그
    FGameplayTag StatusTag;            // 상태 태그 (잠금/해제/장착)
    FGameplayTag AbilityType;          // 타입 (Offensive/Passive)
    FGameplayTag CooldownTag;          // 쿨다운 태그

    TObjectPtr<const UTexture2D> Icon;         // 아이콘
    TObjectPtr<const UMaterialInterface> Background; // 배경

    int32 LevelRequirement;            // 필요 레벨
    TSubclassOf<UGameplayAbility> Ability; // 어빌리티 클래스
};
```

### 6.2 DA_AttributeInfo
속성의 UI 표시 정보를 저장합니다.

**구조:**
```cpp
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
    FGameplayTag AttributeTag;         // 속성 태그
    FText AttributeName;               // 표시 이름
    FText AttributeDescription;        // 설명
    float AttributeValue;              // 값 (런타임에 설정)
};
```

### 6.3 DA_CharacterClassInfo
캐릭터 클래스별 기본 설정을 저장합니다.

**구조:**
```cpp
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
    TSubclassOf<UGameplayEffect> PrimaryAttributes; // 기본 Primary 속성
    TArray<TSubclassOf<UGameplayAbility>> StartupAbilities; // 시작 어빌리티
};
```

**클래스 타입:**
- Elementalist: 마법사 타입
- Warrior: 전사 타입
- Ranger: 원거리 공격 타입

### 6.4 DA_LevelUpInfo
레벨업 시스템 데이터를 저장합니다.

**구조:**
```cpp
USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
    int32 LevelUpRequirement;      // 필요 XP
    int32 AttributePointAward;     // 보상 속성 포인트
    int32 SpellPointAward;         // 보상 스펠 포인트
};
```

### 6.5 DA_LootTiers
루트 드롭 테이블을 정의합니다.

**구조:**
```cpp
USTRUCT(BlueprintType)
struct FLootItem
{
    TSubclassOf<AActor> LootClass;     // 드롭 아이템 클래스
    float ChanceToSpawn;               // 스폰 확률 (0-100)
    int32 MaxNumberToSpawn;            // 최대 스폰 개수
    bool bLootLevelOverride;           // 레벨 오버라이드 여부
};
```

---

## 7. 네트워크 복제

### 7.1 복제 구조

#### 플레이어 캐릭터
- ASC는 **PlayerState**에서 관리
- ReplicationMode: **Mixed** (Full + Minimal)
- 클라이언트에서 예측 실행, 서버에서 확인

#### 적 캐릭터
- ASC는 **캐릭터 자체**에서 관리
- ReplicationMode: **Minimal**
- 서버에서만 실행

### 7.2 주요 복제 속성

```cpp
// AuraCharacterBase
DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
DOREPLIFETIME(AAuraCharacterBase, bIsBurned);
DOREPLIFETIME(AAuraCharacterBase, bIsBeingShocked);

// AuraPlayerState
DOREPLIFETIME(AAuraPlayerState, Level);
DOREPLIFETIME(AAuraPlayerState, XP);
DOREPLIFETIME(AAuraPlayerState, AttributePoints);
DOREPLIFETIME(AAuraPlayerState, SpellPoints);
```

### 7.3 RPC 함수

```cpp
// 서버 RPC (클라이언트 → 서버)
UFUNCTION(Server, Reliable)
void ServerSpendSpellPoint(const FGameplayTag& AbilityTag);

UFUNCTION(Server, Reliable)
void ServerEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Slot);

// 클라이언트 RPC (서버 → 클라이언트)
UFUNCTION(Client, Reliable)
void ClientEquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
    const FGameplayTag& Slot, const FGameplayTag& PreviousSlot);

// 멀티캐스트 (서버 → 모든 클라이언트)
UFUNCTION(NetMulticast, Reliable)
void MulticastHandleDeath(const FVector& DeathImpulse);

UFUNCTION(NetMulticast, Unreliable)
void MulticastLevelUpParticles();
```

### 7.4 타겟 데이터 복제

```cpp
// 클라이언트에서 타겟 데이터 수집 후 서버로 전송
void UTargetDataUnderMouse::SendMouseCursorData()
{
    FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

    // 커서 히트 결과 수집
    FHitResult CursorHit;
    PC->GetHitResultUnderCursor(ECC_Target, false, CursorHit);

    // 서버로 전송
    AbilitySystemComponent->ServerSetReplicatedTargetData(...);
}
```

---

## 부록

### A. GameplayTag 목록

#### 입력 태그
- `InputTag.LMB` - 마우스 왼쪽 버튼
- `InputTag.RMB` - 마우스 오른쪽 버튼
- `InputTag.1` ~ `InputTag.4` - 숫자키
- `InputTag.Passive_1`, `InputTag.Passive_2` - 패시브 슬롯

#### 어빌리티 태그
- `Abilities.Fire.FireBolt` - 파이어볼트
- `Abilities.Fire.FireBlast` - 파이어블라스트
- `Abilities.Lightning.Electrocute` - 일렉트로큐트
- `Abilities.Arcane.ArcaneShards` - 아케인 샤드
- `Abilities.Passive.*` - 패시브 스킬들

#### 데미지 타입 태그
- `Damage.Fire` - 화염 데미지
- `Damage.Lightning` - 번개 데미지
- `Damage.Arcane` - 비전 데미지
- `Damage.Physical` - 물리 데미지

#### 디버프 태그
- `Debuff.Burn` - 화상
- `Debuff.Stun` - 기절
- `Debuff.Arcane` - 비전 디버프
- `Debuff.Physical` - 물리 디버프

#### 상태 태그
- `Abilities.Status.Locked` - 잠김
- `Abilities.Status.Eligible` - 해제 가능
- `Abilities.Status.Unlocked` - 해제됨
- `Abilities.Status.Equipped` - 장착됨

### B. 문제 해결

#### Q: 어빌리티가 활성화되지 않음
1. InputConfig에 어빌리티가 바인딩되어 있는지 확인
2. 어빌리티 상태가 Unlocked 또는 Equipped인지 확인
3. 쿨다운 중인지 확인
4. 마나가 충분한지 확인

#### Q: 데미지가 적용되지 않음
1. DamageEffectParams가 올바르게 설정되었는지 확인
2. TargetASC가 유효한지 확인
3. 아군에게 데미지를 주려는 것인지 확인 (IsNotFriend 체크)

#### Q: 속성 변경이 UI에 반영되지 않음
1. 속성에 OnRep 함수가 있는지 확인
2. 델리게이트 브로드캐스트가 호출되는지 확인
3. WidgetController가 올바르게 바인딩되었는지 확인

#### Q: 네트워크에서 동기화 문제
1. HasAuthority() 체크가 올바른지 확인
2. 복제 설정 (DOREPLIFETIME)이 되어 있는지 확인
3. RPC가 올바른 위치에서 호출되는지 확인

---

*이 문서는 Aura GAS 프로젝트의 주요 시스템을 설명합니다. 추가 질문이 있으면 코드의 주석을 참조하세요.*
