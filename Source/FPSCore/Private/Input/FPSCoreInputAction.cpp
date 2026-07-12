// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/FPSCoreInputConfig.h"

const UInputAction* UFPSCoreInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	if (!InputTag.IsValid())
	{
		return nullptr;
	}
	
	for (const FFPSCoreInputAction& InputAction : NativeInputActions)
	{
		if (IsValid(InputAction.InputAction) && InputAction.InputTag.MatchesTagExact(InputTag))
		{
			return InputAction.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("Could not find native input action for tag [%s] in Input Config [%s]"),*InputTag.ToString(),
			*GetNameSafe(this));
	}
	return nullptr;
}
