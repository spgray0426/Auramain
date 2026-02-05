/*
 * AuraAbilityTypes.cpp
 * Aura 프로젝트의 커스텀 게임플레이 이펙트 컨텍스트 구현
 *
 * 이 파일은 FAuraGameplayEffectContext의 네트워크 직렬화를 구현합니다.
 * 기본 FGameplayEffectContext를 확장하여 추가 데이터를 네트워크로 전송합니다:
 * - 크리티컬 히트 / 블록 여부
 * - 디버프 정보 (데미지, 지속시간, 빈도)
 * - 데미지 유형
 * - 사망 충격량 / 넉백 힘
 * - 광역 데미지 정보
 */

#include "AuraAbilityTypes.h"

/**
 * 게임플레이 이펙트 컨텍스트의 네트워크 직렬화
 * 멀티플레이어 게임에서 서버-클라이언트 간 데이터 동기화에 사용
 *
 * @param Ar - 직렬화 아카이브 (저장/로딩)
 * @param Map - 패키지 맵
 * @param bOutSuccess - 성공 여부 출력
 * @return 직렬화 성공 여부
 *
 * 비트 플래그 구조 (RepBits):
 * - Bit 0: Instigator (공격 시전자)
 * - Bit 1: EffectCauser (효과 원인 액터)
 * - Bit 2: AbilityCDO (어빌리티 클래스 기본 객체)
 * - Bit 3: SourceObject (소스 오브젝트)
 * - Bit 4: Actors (관련 액터 배열)
 * - Bit 5: HitResult (히트 결과)
 * - Bit 6: WorldOrigin (월드 원점)
 * - Bit 7: bIsBlockedHit (블록 히트 여부)
 * - Bit 8: bIsCriticalHit (크리티컬 히트 여부)
 * - Bit 9: bIsSuccessfulDebuff (디버프 적용 성공 여부)
 * - Bit 10: DebuffDamage (디버프 데미지)
 * - Bit 11: DebuffDuration (디버프 지속시간)
 * - Bit 12: DebuffFrequency (디버프 빈도)
 * - Bit 13: DamageType (데미지 유형)
 * - Bit 14: DeathImpulse (사망 시 충격량)
 * - Bit 15: KnockbackForce (넉백 힘)
 * - Bit 16-19: RadialDamage (광역 데미지 정보)
 */
bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// 직렬화할 데이터를 나타내는 비트 플래그
	uint32 RepBits = 0;

	// 저장 모드: 어떤 데이터가 유효한지 비트 플래그 설정
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid())
		{
			RepBits |= 1 << 0;
		}
		if (bReplicateEffectCauser && EffectCauser.IsValid() )
		{
			RepBits |= 1 << 1;
		}
		if (AbilityCDO.IsValid())
		{
			RepBits |= 1 << 2;
		}
		if (bReplicateSourceObject && SourceObject.IsValid())
		{
			RepBits |= 1 << 3;
		}
		if (Actors.Num() > 0)
		{
			RepBits |= 1 << 4;
		}
		if (HitResult.IsValid())
		{
			RepBits |= 1 << 5;
		}
		if (bHasWorldOrigin)
		{
			RepBits |= 1 << 6;
		}
		if (bIsBlockedHit)
		{
			RepBits |= 1 << 7;
		}
		if (bIsCriticalHit)
		{
			RepBits |= 1 << 8;
		}
		if (bIsSuccessfulDebuff)
		{
			RepBits |= 1 << 9;
		}
		if (DebuffDamage > 0.f)
		{
			RepBits |= 1 << 10;
		}
		if (DebuffDuration > 0.f)
		{
			RepBits |= 1 << 11;
		}
		if (DebuffFrequency > 0.f)
		{
			RepBits |= 1 << 12;
		}
		if (DamageType.IsValid())
		{
			RepBits |= 1 << 13;
		}
		if (!DeathImpulse.IsZero())
		{
			RepBits |= 1 << 14;
		}
		if (!KnockbackForce.IsZero())
		{
			RepBits |= 1 << 15;
		}
		if (bIsRadialDamage)
		{
			RepBits |= 1 << 16;

			if (RadialDamageInnerRadius > 0.f)
			{
				RepBits |= 1 << 17;
			}
			if (RadialDamageOuterRadius > 0.f)
			{
				RepBits |= 1 << 18;
			}
			if (!RadialDamageOrigin.IsZero())
			{
				RepBits |= 1 << 19;
			}
		}
		
	}

	// 19비트의 플래그를 직렬화
	Ar.SerializeBits(&RepBits, 19);

	// 각 비트 플래그에 따라 해당 데이터 직렬화/역직렬화
	// Instigator (공격 시전자)
	if (RepBits & (1 << 0))
	{
		Ar << Instigator;
	}
	if (RepBits & (1 << 1))
	{
		Ar << EffectCauser;
	}
	if (RepBits & (1 << 2))
	{
		Ar << AbilityCDO;
	}
	if (RepBits & (1 << 3))
	{
		Ar << SourceObject;
	}
	if (RepBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!HitResult.IsValid())
			{
				HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else
	{
		bHasWorldOrigin = false;
	}
	if (RepBits & (1 << 7))
	{
		Ar << bIsBlockedHit;
	}
	if (RepBits & (1 << 8))
	{
		Ar << bIsCriticalHit;
	}
	if (RepBits & (1 << 9))
	{
		Ar << bIsSuccessfulDebuff;
	}
	if (RepBits & (1 << 10))
	{
		Ar << DebuffDamage;
	}
	if (RepBits & (1 << 11))
	{
		Ar << DebuffDuration;
	}
	if (RepBits & (1 << 12))
	{
		Ar << DebuffFrequency;
	}
	if (RepBits & (1 << 13))
	{
		if (Ar.IsLoading())
		{
			if (!DamageType.IsValid())
			{
				DamageType = TSharedPtr<FGameplayTag>(new FGameplayTag());
			}
		}
		DamageType->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 14))
	{
		DeathImpulse.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 15))
	{
		KnockbackForce.NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 16))
	{
		Ar << bIsRadialDamage;
		
		if (RepBits & (1 << 17))
		{
			Ar << RadialDamageInnerRadius;
		}
		if (RepBits & (1 << 18))
		{
			Ar << RadialDamageOuterRadius;
		}
		if (RepBits & (1 << 19))
		{
			RadialDamageOrigin.NetSerialize(Ar, Map, bOutSuccess);
		}
	}
	

	// 로딩 모드: InstigatorAbilitySystemComponent 초기화
	if (Ar.IsLoading())
	{
		// Instigator의 AbilitySystemComponent를 캐싱하기 위해 호출
		AddInstigator(Instigator.Get(), EffectCauser.Get());
	}

	bOutSuccess = true;
	return true;
}
