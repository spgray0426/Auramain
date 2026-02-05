// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * FAuraGameplayTags
 * 게임플레이 태그 싱글톤
 * 게임에서 사용하는 모든 네이티브 게임플레이 태그를 정의합니다.
 * 속성, 데미지, 어빌리티, 디버프 등의 태그를 관리합니다.
 */

struct FAuraGameplayTags
{
public:
    // 싱글톤 인스턴스 가져오기
    static const FAuraGameplayTags& Get() { return GameplayTags;}

    // 네이티브 게임플레이 태그 초기화
    static void InitializeNativeGameplayTags();

	// === 주요 속성 태그 ===
	FGameplayTag Attributes_Primary_Strength;       // 힘 - 물리 데미지와 방어력 증가
	FGameplayTag Attributes_Primary_Intelligence;   // 지능 - 마법 데미지와 마나량 증가
	FGameplayTag Attributes_Primary_Resilience;     // 회복력 - 방어력과 회복량 증가
	FGameplayTag Attributes_Primary_Vigor;          // 활력 - 체력과 체력 회복 증가

	// === 보조 속성 태그 ===
	FGameplayTag Attributes_Secondary_Armor;                    // 방어력 - 받는 데미지 감소
	FGameplayTag Attributes_Secondary_ArmorPenetration;         // 방어구 관통 - 적 방어력 무시
	FGameplayTag Attributes_Secondary_BlockChance;              // 막기 확률 - 데미지 절반 감소 확률
	FGameplayTag Attributes_Secondary_CriticalHitChance;        // 치명타 확률
	FGameplayTag Attributes_Secondary_CriticalHitDamage;        // 치명타 데미지 배율
	FGameplayTag Attributes_Secondary_CriticalHitResistance;    // 치명타 저항 - 받는 치명타 데미지 감소
	FGameplayTag Attributes_Secondary_HealthRegeneration;       // 체력 재생 - 초당 체력 회복량
	FGameplayTag Attributes_Secondary_ManaRegeneration;         // 마나 재생 - 초당 마나 회복량
	FGameplayTag Attributes_Secondary_MaxHealth;                // 최대 체력
	FGameplayTag Attributes_Secondary_MaxMana;                  // 최대 마나

	// === 메타 속성 태그 ===
	FGameplayTag Attributes_Meta_IncomingXP;                    // 수신 경험치 (전투 후 획득)

	// === 입력 태그 ===
	FGameplayTag InputTag_LMB;          // 좌클릭 - 기본 공격
	FGameplayTag InputTag_RMB;          // 우클릭 - 보조 공격
	FGameplayTag InputTag_1;            // 스킬 슬롯 1
	FGameplayTag InputTag_2;            // 스킬 슬롯 2
	FGameplayTag InputTag_3;            // 스킬 슬롯 3
	FGameplayTag InputTag_4;            // 스킬 슬롯 4
	FGameplayTag InputTag_Passive_1;    // 패시브 슬롯 1
	FGameplayTag InputTag_Passive_2;    // 패시브 슬롯 2

	// === 데미지 타입 태그 ===
	FGameplayTag Damage;                // 기본 데미지 (루트 태그)
	FGameplayTag Damage_Fire;           // 화염 데미지 - 화상 디버프 유발 가능
	FGameplayTag Damage_Lightning;      // 번개 데미지 - 기절 디버프 유발 가능
	FGameplayTag Damage_Arcane;         // 비전 데미지 - 비전 디버프 유발 가능
	FGameplayTag Damage_Physical;       // 물리 데미지 - 물리 디버프 유발 가능

	// === 저항 속성 태그 ===
	FGameplayTag Attributes_Resistance_Fire;        // 화염 저항 - 화염 데미지 감소
	FGameplayTag Attributes_Resistance_Lightning;   // 번개 저항 - 번개 데미지 감소
	FGameplayTag Attributes_Resistance_Arcane;      // 비전 저항 - 비전 데미지 감소
	FGameplayTag Attributes_Resistance_Physical;    // 물리 저항 - 물리 데미지 감소

	// === 디버프 타입 태그 ===
	FGameplayTag Debuff_Burn;           // 화상 - 지속 화염 데미지
	FGameplayTag Debuff_Stun;           // 기절 - 행동 불가
	FGameplayTag Debuff_Arcane;         // 비전 약화 - 마법 저항 감소
	FGameplayTag Debuff_Physical;       // 물리 약화 - 물리 저항 감소

	// === 디버프 속성 태그 ===
	FGameplayTag Debuff_Chance;         // 디버프 발생 확률
	FGameplayTag Debuff_Damage;         // 디버프 틱당 데미지
	FGameplayTag Debuff_Duration;       // 디버프 지속 시간
	FGameplayTag Debuff_Frequency;      // 디버프 틱 주기

	// === 어빌리티 기본 태그 ===
	FGameplayTag Abilities_None;            // 어빌리티 없음

	FGameplayTag Abilities_Attack;          // 기본 공격 어빌리티
	FGameplayTag Abilities_Summon;          // 소환 어빌리티

	FGameplayTag Abilities_HitReact;        // 피격 반응 어빌리티

	// === 어빌리티 상태 태그 ===
	FGameplayTag Abilities_Status_Locked;       // 잠김 - 레벨 요구사항 미충족
	FGameplayTag Abilities_Status_Eligible;     // 획득 가능 - 레벨 충족, 포인트 필요
	FGameplayTag Abilities_Status_Unlocked;     // 잠금 해제 - 획득했으나 미장착
	FGameplayTag Abilities_Status_Equipped;     // 장착됨 - 사용 가능

	// === 어빌리티 타입 태그 ===
	FGameplayTag Abilities_Type_Offensive;      // 공격형 어빌리티
	FGameplayTag Abilities_Type_Passive;        // 패시브 어빌리티
	FGameplayTag Abilities_Type_None;           // 타입 없음

	// === 화염 어빌리티 ===
	FGameplayTag Abilities_Fire_FireBolt;       // 화염구 - 유도 투사체
	FGameplayTag Abilities_Fire_FireBlast;      // 화염 폭발 - 범위 공격

	// === 번개 어빌리티 ===
	FGameplayTag Abilities_Lightning_Electrocute;   // 전기 충격 - 연쇄 번개

	// === 비전 어빌리티 ===
	FGameplayTag Abilities_Arcane_ArcaneShards;     // 비전 파편 - 다중 투사체

	// === 패시브 어빌리티 ===
	FGameplayTag Abilities_Passive_HaloOfProtection;    // 보호의 후광 - 방어력 증가
	FGameplayTag Abilities_Passive_LifeSiphon;          // 생명력 흡수 - 데미지 회복
	FGameplayTag Abilities_Passive_ManaSiphon;          // 마나 흡수 - 마나 회복

	// === 쿨다운 태그 ===
	FGameplayTag Cooldown_Fire_FireBolt;        // 화염구 쿨다운

	// === 전투 소켓 태그 ===
	FGameplayTag CombatSocket_Weapon;       // 무기 소켓 - 무기 투사체 발사 위치
	FGameplayTag CombatSocket_RightHand;    // 오른손 소켓
	FGameplayTag CombatSocket_LeftHand;     // 왼손 소켓
	FGameplayTag CombatSocket_Tail;         // 꼬리 소켓 - 적 전용

	// === 공격 몽타주 태그 ===
	FGameplayTag Montage_Attack_1;          // 공격 애니메이션 1
	FGameplayTag Montage_Attack_2;          // 공격 애니메이션 2
	FGameplayTag Montage_Attack_3;          // 공격 애니메이션 3
	FGameplayTag Montage_Attack_4;          // 공격 애니메이션 4

	// === 데미지 타입 매핑 ===
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;  // 데미지 타입 → 저항 매핑
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;      // 데미지 타입 → 디버프 매핑

	// === 이펙트 태그 ===
	FGameplayTag Effects_HitReact;          // 피격 반응 이펙트

	// === 플레이어 입력 차단 태그 ===
	FGameplayTag Player_Block_InputPressed;     // 입력 시작 차단
	FGameplayTag Player_Block_InputHeld;        // 입력 유지 차단
	FGameplayTag Player_Block_InputReleased;    // 입력 해제 차단
	FGameplayTag Player_Block_CursorTrace;      // 커서 추적 차단

	// === 게임플레이 큐 태그 ===
	FGameplayTag GameplayCue_FireBlast;         // 화염 폭발 시각/사운드 효과

private:
    static FAuraGameplayTags GameplayTags;
};
