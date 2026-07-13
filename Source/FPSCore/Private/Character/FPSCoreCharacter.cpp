// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCore/Public/Character/FPSCoreCharacter.h"

#include "EnhancedInputComponent.h"
#include "Character/FPSCorePlayerController.h"
#include "Components/FPSCoreHealthComponent.h"
#include "FPSCore/Public/Character/FPSCorePlayerState.h"
#include "Input/FPSCoreGameplayTags.h"
#include "Input/FPSCoreInputBinding.h"
#include "Input/FPSCoreInputConfig.h"


AFPSCoreCharacter::AFPSCoreCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	HealthComponent = CreateDefaultSubobject<UFPSCoreHealthComponent>("HealthComponent");
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
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!IsValid(EnhancedInputComponent))
	{
		UE_LOG(LogTemp,Error,TEXT("%s requires UEnhancedInputComponent"),*GetNameSafe(this));
		return;
	}
	
	const AFPSCorePlayerController* FPSController = Cast<AFPSCorePlayerController>(GetController());

	if (!IsValid(FPSController))
	{
		UE_LOG(LogTemp,Error,TEXT("%s requires AFPSCorePlayerController"),*GetNameSafe(this));
		return;
	}
	
	const UFPSCoreInputConfig* InputConfig = FPSController->GetInputConfig();

	if (!IsValid(InputConfig))
	{
		UE_LOG(LogTemp,Error,TEXT("%s does not have a valid Input Config"),*GetNameSafe(FPSController));
		return;
	}
	
	FPSCoreInput::BindNativeAction(EnhancedInputComponent,InputConfig,FPSCoreGameplayTags::Input_Look,
		ETriggerEvent::Triggered,this,&ThisClass::Input_Look);
	FPSCoreInput::BindNativeAction(EnhancedInputComponent,InputConfig,FPSCoreGameplayTags::Input_Move,
		ETriggerEvent::Triggered,this,&ThisClass::Input_Move);
	FPSCoreInput::BindNativeAction(EnhancedInputComponent,InputConfig,FPSCoreGameplayTags::Input_Crouch,
		ETriggerEvent::Triggered,this,&ThisClass::Input_Crouch);
	FPSCoreInput::BindNativeAction(EnhancedInputComponent,InputConfig,FPSCoreGameplayTags::Input_Jump,
		ETriggerEvent::Triggered,this,&ThisClass::Input_Jump);
	
	FPSCoreInput::BindAbilityActions(EnhancedInputComponent,InputConfig,this,&ThisClass::Input_AbilityTagPressed,
		&ThisClass::Input_AbilityTagReleased);
	
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

	if (!IsValid(FPSPlayerState))
	{
		UE_LOG(LogTemp,Warning,TEXT("%s does not have FPSCorePlayerState."),*GetNameSafe(this));
		return;
	}
	
	FPSPlayerState->InitializeAbilitySystem(this);

	if (IsValid(HealthComponent))
	{
		HealthComponent->InitializeWithAbilitySystem(FPSPlayerState->GetFPSCoreAbilitySystemComponent(),FPSPlayerState->GetHealthSet());
	}
}

UAbilitySystemComponent* AFPSCoreCharacter::GetAbilitySystemComponent() const
{
	return GetFPSCoreAbilitySystemComponent();
}

UFPSCoreAbilitySystemComponent* AFPSCoreCharacter::GetFPSCoreAbilitySystemComponent() const
{
	const AFPSCorePlayerState* FPSPlayerState = GetPlayerState<AFPSCorePlayerState>();

	if (!IsValid(FPSPlayerState))
	{
		return nullptr;
	}
	
	return FPSPlayerState->GetFPSCoreAbilitySystemComponent();
}

void AFPSCoreCharacter::Input_Move(const FInputActionValue& Value)
{
	if (!IsValid(Controller))
	{
		return;
	}
	
	const FVector2D MovementValue = Value.Get<FVector2D>();
	const FRotator ControlRotation = Controller->GetControlRotation();
	
	const FRotator YawRotation(0.0,ControlRotation.Yaw,0.0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection,MovementValue.Y);
	AddMovementInput(RightDirection,MovementValue.X);
}

void AFPSCoreCharacter::Input_Look(const FInputActionValue& Value)
{
	const FVector2D LookValue = Value.Get<FVector2D>();

	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void AFPSCoreCharacter::Input_AbilityTagPressed(FGameplayTag InputTag)
{
	UFPSCoreAbilitySystemComponent* ASC = GetFPSCoreAbilitySystemComponent();
	if (IsValid(ASC) && InputTag.IsValid())
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void AFPSCoreCharacter::Input_AbilityTagReleased(FGameplayTag InputTag)
{
	UFPSCoreAbilitySystemComponent* ASC = GetFPSCoreAbilitySystemComponent();
	if (IsValid(ASC) && InputTag.IsValid())
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void AFPSCoreCharacter::UnPossessed()
{
	if (IsValid(HealthComponent))
	{
		HealthComponent->UninitializeFromAbilitySystem();
	}
	
	if (UFPSCoreAbilitySystemComponent* ASC = GetFPSCoreAbilitySystemComponent())
	{
		ASC->ClearAbilityInput();
	}
	Super::UnPossessed();
}

