#include "FPSCoreGameplayAbility.h"

#include "Character/FPSCorePlayerState.h"

AFPSCorePlayerState* UFPSCoreGameplayAbility::GetFPSCorePlayerState() const
{
	return Cast<AFPSCorePlayerState>(GetOwningActorFromActorInfo());
}

int32 UFPSCoreGameplayAbility::GetOwningPlayerId() const
{
	const AFPSCorePlayerState* FPSPlayerState = GetFPSCorePlayerState();

	if (!IsValid(FPSPlayerState))
	{
		return INDEX_NONE;
	}
	return FPSPlayerState->GetPlayerId();
}
