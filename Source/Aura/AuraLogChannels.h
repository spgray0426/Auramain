#pragma once

#include "CoreMinimal.h"
#include "Logging/LogMacros.h"

/**
 * LogAura
 * Aura 로그 카테고리
 * 게임 전용 디버그 로그를 출력하는 데 사용됩니다.
 * 사용 예: UE_LOG(LogAura, Warning, TEXT("메시지"));
 */
DECLARE_LOG_CATEGORY_EXTERN(LogAura, Log, All);