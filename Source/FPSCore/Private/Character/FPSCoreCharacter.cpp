// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCore/Public/Character/FPSCoreCharacter.h"

#include "FPSCore/Public/Character/FPSCorePlayerState.h"


AFPSCoreCharacter::AFPSCoreCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AFPSCoreCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSCoreCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFPSCoreCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFPSCoreCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeAbilitySystem();
}

void AFPSCoreCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitializeAbilitySystem();
}

void AFPSCoreCharacter::InitializeAbilitySystem()
{
	AFPSCorePlayerState* FPSPlayerState = GetPlayerState<AFPSCorePlayerState>();

	if (IsValid(FPSPlayerState))
	{
		UE_LOG(LogTemp,Warning,TEXT("%s does not have FPSCorePlayerState."),*GetNameSafe(this));
		return;
	}
	
	FPSPlayerState->InitializeAbilitySystem(this);
}

UAbilitySystemComponent* AFPSCoreCharacter::GetAbilitySystemComponent() const
{
	return GetFPSCoreAbilitySystemComponent();
}

UFPSCoreAbilitySystemComponent* AFPSCoreCharacter::GetFPSCoreAbilitySystemComponent() const
{
	const AFPSCorePlayerState* FPSPlayerState = GetPlayerState<AFPSCorePlayerState>();

	if (IsValid(FPSPlayerState))
	{
		return nullptr;
	}
	
	return FPSPlayerState->GetFPSCoreAbilitySystemComponent();
}
