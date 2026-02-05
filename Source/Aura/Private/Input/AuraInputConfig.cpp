// Copyright Druid Mechanics

/**
 * AuraInputConfig.cpp
 *
 * 입력 설정 데이터 에셋 클래스 구현 파일
 *
 * 게임플레이 태그(GameplayTag)와 입력 액션(InputAction)의 매핑을 정의합니다.
 * 블루프린트에서 데이터 에셋으로 생성하여 에디터에서 설정할 수 있습니다.
 *
 * 구조:
 * - FAuraInputAction: 태그-입력액션 쌍을 저장하는 구조체
 * - AbilityInputActions: 어빌리티 관련 입력 매핑 배열
 *
 * 사용 예:
 * - InputTag.Ability.LMB -> IA_LeftMouseButton
 * - InputTag.Ability.1 -> IA_AbilitySlot1
 */

#include "Input/AuraInputConfig.h"

/**
 * 게임플레이 태그로 입력 액션 찾기
 * @param InputTag - 찾을 게임플레이 태그
 * @param bLogNotFound - 찾지 못했을 때 로그 출력 여부
 * @return 해당 태그에 매핑된 InputAction (없으면 nullptr)
 *
 * AbilityInputActions 배열에서 태그가 일치하는 항목을 검색합니다.
 */
const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FAuraInputAction& Action: AbilityInputActions)
	{
		// InputAction이 유효하고 태그가 일치하는지 확인
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	// 찾지 못한 경우 로그 출력
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
