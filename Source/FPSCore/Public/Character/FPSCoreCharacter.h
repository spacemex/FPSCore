// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "FPSCoreCharacter.generated.h"

class UFPSCoreAbilitySystemComponent;
class UAbilitySystemComponent;

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
	
private:
	void InitializeAbilitySystem();
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
