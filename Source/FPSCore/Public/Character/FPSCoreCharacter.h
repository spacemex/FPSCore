// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "FPSCoreCharacter.generated.h"

class UFPSCoreAbilitySystemComponent;
class UAbilitySystemComponent;
class AFPSCorePlayerState;
struct FInputActionValue;

UCLASS()
class FPSCORE_API AFPSCoreCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AFPSCoreCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Abilities")
	UFPSCoreAbilitySystemComponent* GetFPSCoreAbilitySystemComponent() const;

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void UnPossessed() override;
private:
	void Input_Move(const FInputActionValue& Value);
	void Input_Look(const FInputActionValue& Value);
	void Input_Jump(const FInputActionValue& Value) {};
	void Input_Crouch(const FInputActionValue& Value){};
	
	void Input_AbilityTagPressed(FGameplayTag InputTag);
	void Input_AbilityTagReleased(FGameplayTag InputTag);
	
	void InitializeAbilitySystem();
	
public:
	virtual void Tick(float DeltaTime) override;

};
