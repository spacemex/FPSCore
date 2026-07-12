// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCore/Public/Character/FPSCorePlayerState.h"

AFPSCorePlayerState::AFPSCorePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFPSCoreAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AFPSCorePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UFPSCoreAbilitySystemComponent* AFPSCorePlayerState::GetFPSCoreAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFPSCorePlayerState::InitializeAbilitySystem(AActor* NewAvatarActor)
{
	if (!IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp,Error,TEXT("AFPSCorePlayerState has not valid AbilitySystemComponent."));
		return;
	}

	if (!IsValid(NewAvatarActor))
	{
		UE_LOG(LogTemp,Warning,TEXT("Cannot initialize Ability System without a valid Avatar Actor."));
		return;
	}
	
	AbilitySystemComponent->InitAbilityActorInfo(this,NewAvatarActor);
	
	if (HasAuthority() && !bDefaultAbilitySetGranted && IsValid(DefaultAbilitySet))
	{
		DefaultAbilitySet->GiveToAbilitySystem(AbilitySystemComponent,&DefaultAbilitySetHandles,this);
		bDefaultAbilitySetGranted = true;
	}
}
