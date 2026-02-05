# Aura GAS 빠른 참조 가이드

## 핵심 클래스 위치

| 클래스 | 파일 경로 |
|--------|----------|
| UAuraAbilitySystemComponent | AbilitySystem/AuraAbilitySystemComponent.h |
| UAuraAttributeSet | AbilitySystem/AuraAttributeSet.h |
| UAuraAbilitySystemLibrary | AbilitySystem/AuraAbilitySystemLibrary.h |
| UExecCalc_Damage | AbilitySystem/ExecCalc/ExecCalc_Damage.h |
| AAuraCharacterBase | Character/AuraCharacterBase.h |
| AAuraPlayerController | Player/AuraPlayerController.h |

---

## 자주 사용하는 코드 패턴

### GameplayEffect 적용
```cpp
FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
ContextHandle.AddSourceObject(this);
FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, Level, ContextHandle);
ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
```

### 태그로 어빌리티 트리거
```cpp
FGameplayTagContainer TagContainer;
TagContainer.AddTag(FAuraGameplayTags::Get().Abilities_Fire_FireBolt);
ASC->TryActivateAbilitiesByTag(TagContainer);
```

### 속성값 가져오기
```cpp
// AttributeSet에서 직접
float Health = AttributeSet->GetHealth();

// ASC를 통해
bool bFound;
float Health = ASC->GetGameplayAttributeValue(UAuraAttributeSet::GetHealthAttribute(), bFound);
```

### 데미지 적용
```cpp
FDamageEffectParams Params;
Params.WorldContextObject = this;
Params.DamageGameplayEffectClass = DamageEffectClass;
Params.SourceAbilitySystemComponent = GetASC();
Params.TargetAbilitySystemComponent = TargetASC;
Params.BaseDamage = 50.f;
Params.AbilityLevel = GetAbilityLevel();
UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);
```

### 태그 체크
```cpp
// 태그 보유 여부
if (ASC->HasMatchingGameplayTag(FAuraGameplayTags::Get().Debuff_Stun))
{
    // 기절 상태
}

// 태그 이벤트 등록
ASC->RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
    .AddUObject(this, &ThisClass::OnTagChanged);
```

### 어빌리티 정보 조회
```cpp
UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
```

---

## 데미지 계산 공식

```
최종 데미지 = 기본 데미지
    × (1 - 저항력 / 100)
    × (블록 성공 ? 0.5 : 1.0)
    × (1 - 유효방어력 × 방어력계수 / 100)
    × (크리티컬 성공 ? 2.0 + 크리티컬데미지 : 1.0)
```

**유효 방어력:**
```
유효방어력 = 타겟방어력 × (100 - 방어관통 × 관통계수) / 100
```

---

## 속성 계산 공식

### 최대 체력
```
MaxHealth = 80 + (2.5 × Vigor) + (10 × Level)
```

### 최대 마나
```
MaxMana = 50 + (2.5 × Intelligence) + (15 × Level)
```

---

## 주요 GameplayTag

### 입력
| 태그 | 설명 |
|------|------|
| InputTag.LMB | 마우스 왼쪽 |
| InputTag.RMB | 마우스 오른쪽 |
| InputTag.1~4 | 숫자키 |

### 데미지 타입
| 태그 | 저항 태그 |
|------|----------|
| Damage.Fire | Attributes.Resistance.Fire |
| Damage.Lightning | Attributes.Resistance.Lightning |
| Damage.Arcane | Attributes.Resistance.Arcane |
| Damage.Physical | Attributes.Resistance.Physical |

### 어빌리티 상태
| 태그 | 설명 |
|------|------|
| Abilities.Status.Locked | 잠김 |
| Abilities.Status.Eligible | 해제 가능 |
| Abilities.Status.Unlocked | 해제됨 |
| Abilities.Status.Equipped | 장착됨 |

### 디버프
| 태그 | 설명 |
|------|------|
| Debuff.Burn | 화상 (지속 데미지) |
| Debuff.Stun | 기절 (행동 불가) |

---

## 데이터 에셋

| 에셋 | 용도 |
|------|------|
| DA_AbilityInfo | 어빌리티 메타데이터 |
| DA_AttributeInfo | 속성 UI 정보 |
| DA_CharacterClassInfo | 클래스별 기본값 |
| DA_LevelUpInfo | 레벨업 테이블 |
| DA_LootTiers | 루트 드롭 테이블 |

---

## 네트워크 규칙

### 서버에서만 실행
- 어빌리티 부여/제거
- GameplayEffect 적용
- 데미지 계산
- AI 로직

### 클라이언트에서 예측
- 어빌리티 활성화
- 이동 입력
- UI 업데이트

### 복제 대상
- 속성값 (Attributes)
- 어빌리티 Spec
- 상태 플래그 (bIsStunned 등)

---

## 디버깅

### 콘솔 명령어
```
showdebug abilitysystem          // ASC 디버그 표시
AbilitySystem.Debug.NextTarget   // 다음 타겟 디버그
```

### 로그 카테고리
```cpp
UE_LOG(LogAura, Warning, TEXT("메시지"));
```

### 유용한 체크
```cpp
// 서버/클라이언트 확인
if (HasAuthority()) { /* 서버 */ }

// 로컬 컨트롤러 확인
if (IsLocallyControlled()) { /* 로컬 */ }

// ASC 유효성
if (IsValid(ASC)) { /* 유효 */ }
```

---

## 새 기능 추가 체크리스트

### 새 어빌리티
- [ ] 어빌리티 클래스 생성
- [ ] GameplayTag 추가 (AuraGameplayTags.cpp)
- [ ] DA_AbilityInfo에 등록
- [ ] 입력 바인딩 (필요시)
- [ ] GameplayEffect 생성 (필요시)
- [ ] GameplayCue 생성 (필요시)

### 새 속성
- [ ] AttributeSet에 속성 추가
- [ ] OnRep 함수 작성
- [ ] DOREPLIFETIME 등록
- [ ] GameplayTag 추가
- [ ] DA_AttributeInfo에 등록
- [ ] UI 바인딩

### 새 적 캐릭터
- [ ] BP 생성 (BP_EnemyBase 상속)
- [ ] 메시/애니메이션 설정
- [ ] CharacterClass 지정
- [ ] BehaviorTree 설정
- [ ] 공격 몽타주 설정
