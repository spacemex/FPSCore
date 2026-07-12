// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FPSCorePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Character/FPSCorePlayerState.h"
#include "Input/FPSCoreInputConfig.h"

void AFPSCorePlayerController::BeginPlay()
{
	Super::BeginPlay();
	ApplyInputMappings();
}

void AFPSCorePlayerController::ApplyInputMappings()
{
	if (!IsLocalController() || !IsValid(InputConfig))
	{
		return;
	}
	
	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (!IsValid(LocalPlayer))
	{
		return;
	}
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	if (!IsValid(InputSubsystem))
	{
		return;
	}
	
	for (const FFPSCoreInputMappingContext& Mapping : InputConfig->MappingContexts)
	{
		if (!IsValid(Mapping.InputMappingContext))
		{
			continue;
		}
		
		InputSubsystem->RemoveMappingContext(Mapping.InputMappingContext);
		InputSubsystem->AddMappingContext(Mapping.InputMappingContext, Mapping.Priority);
	}
}

void AFPSCorePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	Super::PostProcessInput(DeltaTime, bGamePaused);

	if (!IsLocalController())
	{
		return;
	}
	
	const AFPSCorePlayerState* FPSPlayerState = GetPlayerState<AFPSCorePlayerState>();

	if (!IsValid(FPSPlayerState))
	{
		return;
	}
	
	UFPSCoreAbilitySystemComponent* ASC = FPSPlayerState->GetFPSCoreAbilitySystemComponent();

	if (!IsValid(ASC))
	{
		return;
	}
	
	ASC->ProcessAbilityInput(DeltaTime,bGamePaused);
}
