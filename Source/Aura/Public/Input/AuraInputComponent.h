// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * UAuraInputComponent
 * Aura 커스텀 입력 컴포넌트
 * 향상된 입력 시스템을 사용하여 어빌리티 입력을 바인딩합니다.
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	/**
	 * 어빌리티 액션 바인딩
	 * 입력 설정의 모든 어빌리티 입력을 바인딩합니다.
	 * @param InputConfig 입력 설정 데이터 에셋
	 * @param Object 콜백 함수를 호출할 객체
	 * @param PressedFunc 입력 시작 시 호출될 함수
	 * @param ReleasedFunc 입력 종료 시 호출될 함수
	 * @param HeldFunc 입력 유지 중 호출될 함수
	 */
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);

	// 모든 어빌리티 입력 액션을 순회하며 바인딩
	for (const FAuraInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// 입력 시작 (키 누름)
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			// 입력 종료 (키 뗌)
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			// 입력 유지 중 (키를 누르고 있음)
			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
