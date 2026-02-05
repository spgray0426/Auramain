// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"

/**
 * Aura 전역 정의
 * 게임 전체에서 사용하는 커스텀 뎁스 스텐실 값과 콜리전 채널을 정의합니다.
 */

// === 커스텀 뎁스 스텐실 값 ===
#define CUSTOM_DEPTH_RED 250    // 빨강 (적 하이라이트)
#define CUSTOM_DEPTH_BLUE 251   // 파랑 (우호적 하이라이트)
#define CUSTOM_DEPTH_TAN 252    // 황갈색 (상호작용 가능 오브젝트)

// === 콜리전 채널 ===
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1        // 투사체 채널
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2            // 타겟팅 채널
#define ECC_ExcludePlayers ECollisionChannel::ECC_GameTraceChannel3    // 플레이어 제외 채널