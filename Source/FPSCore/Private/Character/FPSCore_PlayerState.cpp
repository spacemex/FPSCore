// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCore/GAS/Attributes/FPSCoreHealthSet.h"
#include "FPSCore/Public/Character/FPSCorePlayerState.h"
#include "GameplayEffect.h"

AFPSCorePlayerState::AFPSCorePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UFPSCoreAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	HealthSet = CreateDefaultSubobject<UFPSCoreHealthSet>(TEXT("HealthSet"));
	
	AbilitySystemComponent->AddAttributeSetSubobject(HealthSet.Get());
	
	SetNetUpdateFrequency(100.0f);
	SetMinNetUpdateFrequency(33.0f);
}

UAbilitySystemComponent* AFPSCorePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UFPSCoreAbilitySystemComponent* AFPSCorePlayerState::GetFPSCoreAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AFPSCorePlayerState::InitializeAbilitySystem(
	AActor* NewAvatarActor
)
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(NewAvatarActor))
	{
		return;
	}

	const bool bIsNewAvatar = LastInitializedAvatar.Get() != NewAvatarActor;

	AbilitySystemComponent->InitAbilityActorInfo(this,NewAvatarActor);


	if (HasAuthority() &&!bDefaultAbilitySetGranted && IsValid(DefaultAbilitySet))
	{
		DefaultAbilitySet->GiveToAbilitySystem(AbilitySystemComponent,&DefaultAbilitySetHandles,this);

		bDefaultAbilitySetGranted = true;
	}
	
	if (HasAuthority() && bIsNewAvatar)
	{
		ApplyDefaultAttributes();
		LastInitializedAvatar = NewAvatarActor;
	}
}

void AFPSCorePlayerState::ApplyDefaultAttributes()
{
	if (!IsValid(AbilitySystemComponent) || !IsValid(DefaultAttributesEffect))
	{
		UE_LOG(LogTemp,Warning,TEXT("%s does not have a valid DefaultAttributesEffect."),*GetNameSafe(this));
		return;
	}

	FGameplayEffectContextHandle EffectContext =
		AbilitySystemComponent->MakeEffectContext();

	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle EffectSpec =
		AbilitySystemComponent->MakeOutgoingSpec(
			DefaultAttributesEffect,
			1.0f,
			EffectContext
		);

	if (!EffectSpec.IsValid())
	{
		UE_LOG(LogTemp,Error,TEXT("Failed to create the default attribute effect spec for %s."),*GetNameSafe(this));
		return;
	}

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data.Get());
}
