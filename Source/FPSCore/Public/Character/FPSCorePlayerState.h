// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "FPSCore/GAS/FPSCoreAbilitySet.h"
#include "GameFramework/PlayerState.h"
#include "FPSCorePlayerState.generated.h"

class UFPSCoreAbilitySystemComponent;
class UAbilitySystemComponent;

UCLASS()
class FPSCORE_API AFPSCorePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AFPSCorePlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Abilities")
	UFPSCoreAbilitySystemComponent* GetFPSCoreAbilitySystemComponent() const;
	
	void InitializeAbilitySystem(AActor* NewAvatarActor);
	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="FPS Core|Abilities",meta=(AllowPrivateAccess="true"))
	TObjectPtr<UFPSCoreAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FPS Core|Abilities")
	TObjectPtr<UFPSCoreAbilitySet> DefaultAbilitySet;
	
private:
	UPROPERTY(Transient)
	FFPSCoreAbilitySet_GrantedHandles DefaultAbilitySetHandles;
	
	bool bDefaultAbilitySetGranted = false;
};
