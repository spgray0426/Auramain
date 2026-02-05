/*
 * AuraLogChannels.cpp
 * Aura 프로젝트 전용 로그 채널 정의
 *
 * 이 파일은 Aura 프로젝트에서 사용하는 커스텀 로그 카테고리를 정의합니다.
 * UE_LOG(LogAura, ...) 형태로 사용하여 Aura 관련 로그를 별도로 필터링할 수 있습니다.
 */

#include "AuraLogChannels.h"

// LogAura 로그 카테고리 정의
// 출력 로그 창에서 "LogAura"로 필터링하여 Aura 관련 로그만 확인 가능
DEFINE_LOG_CATEGORY(LogAura);