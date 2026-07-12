// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "FPSCoreInputConfig.h"

namespace FPSCoreInput
{
	template<class UserClass,typename FuncType> void BindNativeAction(
		UEnhancedInputComponent* InputComponent, const UFPSCoreInputConfig* InputConfig, const FGameplayTag& InputTag,
		const ETriggerEvent& TriggerEvent, UserClass* Object, FuncType Function)
	{
		if (!IsValid(InputComponent) || !IsValid(InputConfig) || !IsValid(Object))
		{
			return;
		}
		
		const UInputAction* InputAction = InputConfig->FindNativeInputActionForTag(InputTag);
		if (!IsValid(InputAction))
		{
			return;
		}
		
		InputComponent->BindAction(InputAction,TriggerEvent,Object,Function);
	}
	
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType> void BindAbilityActions(
		UEnhancedInputComponent* InputComponent, const UFPSCoreInputConfig* InputConfig, UserClass* Object,
		PressedFuncType PressedFunction, ReleasedFuncType ReleasedFunction)
	{
		if (!IsValid(InputComponent) || !IsValid(InputConfig) || !IsValid(Object))
		{
			return;
		}
		
		for (const FFPSCoreInputAction& Action : InputConfig->AbilityInputActions)
		{
			if (!IsValid(Action.InputAction) || !Action.InputTag.IsValid())
			{
				continue;
			}
			
			InputComponent->BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunction,Action.InputTag);
			InputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunction,Action.InputTag);
			InputComponent->BindAction(Action.InputAction, ETriggerEvent::Canceled, Object, ReleasedFunction,Action.InputTag);
		}
	}
}
