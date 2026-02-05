// Copyright Druid Mechanics

/**
 * AuraPlayerState.cpp
 *
 * 플레이어 상태(PlayerState) 클래스 구현 파일
 *
 * 이 클래스는 플레이어의 지속적인 게임 상태를 관리합니다.
 * - 어빌리티 시스템 컴포넌트(ASC) 및 속성 집합(AttributeSet) 소유
 * - 플레이어 레벨, 경험치(XP) 관리
 * - 속성 포인트(Attribute Points) 및 스펠 포인트(Spell Points) 관리
 * - 네트워크 복제(Replication)를 통한 멀티플레이어 동기화
 *
 * GAS(Gameplay Ability System)에서 플레이어 캐릭터의 ASC를
 * PlayerState에 배치하면 캐릭터가 리스폰되어도 어빌리티 상태가 유지됩니다.
 */

#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

/**
 * 생성자
 * 어빌리티 시스템 컴포넌트와 속성 집합을 생성하고 초기화합니다.
 * - ASC 복제 모드를 Mixed로 설정 (소유 클라이언트에게 GameplayEffects 복제)
 * - 네트워크 업데이트 주기를 100Hz로 설정하여 빠른 동기화 지원
 */
AAuraPlayerState::AAuraPlayerState()
{
	// 어빌리티 시스템 컴포넌트 생성 및 설정
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	// Mixed 모드: GameplayEffects는 소유 클라이언트에게만, GameplayCues와 Tags는 모두에게 복제
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 속성 집합 생성
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	// 네트워크 업데이트 빈도 설정 (초당 100회)
	NetUpdateFrequency = 100.f;
}

/**
 * 복제할 속성 등록
 * 멀티플레이어에서 서버와 클라이언트 간에 동기화할 변수들을 지정합니다.
 */
void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 레벨, 경험치, 속성 포인트, 스펠 포인트를 복제 대상으로 등록
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

/**
 * 어빌리티 시스템 컴포넌트 반환
 * IAbilitySystemInterface 인터페이스 구현
 */
UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

/**
 * 경험치 추가
 * @param InXP - 추가할 경험치 양
 * 경험치를 누적하고 델리게이트를 통해 UI 등에 알립니다.
 */
void AAuraPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

/**
 * 레벨 추가
 * @param InLevel - 추가할 레벨 수
 * 레벨업 시 사용되며, bLevelUp=true로 브로드캐스트하여 레벨업 이펙트를 트리거합니다.
 */
void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
}

/**
 * 경험치 설정 (직접 값 지정)
 * @param InXP - 설정할 경험치 값
 * 세이브/로드 시 사용됩니다.
 */
void AAuraPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

/**
 * 레벨 설정 (직접 값 지정)
 * @param InLevel - 설정할 레벨 값
 * 세이브/로드 시 사용되며, bLevelUp=false로 레벨업 이펙트를 트리거하지 않습니다.
 */
void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
}

/**
 * 속성 포인트 설정
 * @param InPoints - 설정할 속성 포인트 값
 */
void AAuraPlayerState::SetAttributePoints(int32 InPoints)
{
	AttributePoints = InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

/**
 * 스펠 포인트 설정
 * @param InPoints - 설정할 스펠 포인트 값
 */
void AAuraPlayerState::SetSpellPoints(int32 InPoints)
{
	SpellPoints = InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

/**
 * 레벨 복제 콜백 (서버 -> 클라이언트)
 * 서버에서 레벨이 변경되면 클라이언트에서 호출됩니다.
 */
void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

/**
 * 경험치 복제 콜백 (서버 -> 클라이언트)
 */
void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

/**
 * 속성 포인트 복제 콜백 (서버 -> 클라이언트)
 */
void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

/**
 * 스펠 포인트 복제 콜백 (서버 -> 클라이언트)
 */
void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

/**
 * 속성 포인트 추가
 * @param InPoints - 추가할 속성 포인트 수
 * 레벨업 시 획득하는 속성 포인트를 누적합니다.
 */
void AAuraPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

/**
 * 스펠 포인트 추가
 * @param InPoints - 추가할 스펠 포인트 수
 * 레벨업 시 획득하는 스펠 포인트를 누적합니다.
 */
void AAuraPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
