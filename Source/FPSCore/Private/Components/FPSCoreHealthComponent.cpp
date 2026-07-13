// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FPSCoreHealthComponent.h"

#include "FPSCore/GAS/FPSCoreAbilitySystemComponent.h"
#include "FPSCore/GAS/Attributes/FPSCoreHealthSet.h"
#include "Net/UnrealNetwork.h"


UFPSCoreHealthComponent::UFPSCoreHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UFPSCoreHealthComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UFPSCoreHealthComponent,DeathState);
}

void UFPSCoreHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UninitializeFromAbilitySystem();
	Super::EndPlay(EndPlayReason);
}

void UFPSCoreHealthComponent::InitializeWithAbilitySystem(UFPSCoreAbilitySystemComponent* InAbilitySystemComponent, UFPSCoreHealthSet* InHealthSet)
{
	if (!IsValid(InAbilitySystemComponent) || !IsValid(InHealthSet))
	{
		UE_LOG(LogTemp, Error, TEXT("%s cannot initialize without a valid AbilitySystemComponent and HealthSet"), *GetName());
		return;
	}

	if (AbilitySystemComponent == InAbilitySystemComponent && HealthSet == InHealthSet)
	{
		return;
	}
	
	UninitializeFromAbilitySystem();
	
	AbilitySystemComponent = InAbilitySystemComponent;
	HealthSet = InHealthSet;
	
	HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFPSCoreHealthSet::GetHealthAttribute())
	.AddUObject(this,&ThisClass::HandHealthChanged);
	
	MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFPSCoreHealthSet::GetMaxHealthAttribute())
	.AddUObject(this,&ThisClass::HandMaxHealthChanged);
	
	const float CurrentHealth = HealthSet->GetHealth();
	const float CurrentMaxHealth = HealthSet->GetMaxHealth();
	
	OnMaxHealthChanged.Broadcast(this,CurrentMaxHealth,CurrentMaxHealth);
	OnHealthChanged.Broadcast(this,CurrentHealth,CurrentHealth);

	if (GetOwner()->HasAuthority() && CurrentHealth <= 0.0f)
	{
		StartDeath();
	}
}

void UFPSCoreHealthComponent::UninitializeFromAbilitySystem()
{
	if (IsValid(AbilitySystemComponent))
	{
		if (HealthChangedDelegateHandle.IsValid())
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFPSCoreHealthSet::GetHealthAttribute())
			.Remove(HealthChangedDelegateHandle);
		}
		if (MaxHealthChangedDelegateHandle.IsValid())
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UFPSCoreHealthSet::GetMaxHealthAttribute())
			.Remove(MaxHealthChangedDelegateHandle);
		}
	}
	
	HealthChangedDelegateHandle.Reset();
	MaxHealthChangedDelegateHandle.Reset();
	
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

float UFPSCoreHealthComponent::GetHealth() const
{
	return IsValid(HealthSet) ? HealthSet->GetHealth() : 0.0f;
}

float UFPSCoreHealthComponent::GetMaxHealth() const
{
	return IsValid(HealthSet) ? HealthSet->GetMaxHealth() : 0.0f;
}

float UFPSCoreHealthComponent::GetHealthNormalized() const
{
	const float CurrentMaxHealth = GetMaxHealth();

	if (CurrentMaxHealth <= 0.f)
	{
		return 0.0f;
	}
	
	return FMath::Clamp(GetHealth() / CurrentMaxHealth, 0.0f, 1.0f);
}

void UFPSCoreHealthComponent::HandHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChanged.Broadcast(this,ChangeData.OldValue,ChangeData.NewValue);

	if (GetOwner()->HasAuthority() && ChangeData.OldValue > 0.f && ChangeData.NewValue <= 0.f)
	{
		StartDeath();
	}
}

void UFPSCoreHealthComponent::HandMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnMaxHealthChanged.Broadcast(this, ChangeData.OldValue,ChangeData.NewValue);
}

void UFPSCoreHealthComponent::StartDeath()
{
	if (!GetOwner()->HasAuthority() || DeathState != EFPSCoreDeathState::NotDead)
	{
		return;
	}
	
	SetDeathState(EFPSCoreDeathState::DeathStarted);
}

void UFPSCoreHealthComponent::FinishDeath()
{
	if (!GetOwner()->HasAuthority() || DeathState != EFPSCoreDeathState::DeathStarted)
	{
		return;
	}
	
	SetDeathState(EFPSCoreDeathState::DeathFinished);
}

void UFPSCoreHealthComponent::SetDeathState(EFPSCoreDeathState NewDeathState)
{
	if (DeathState == NewDeathState)
	{
		return;
	}
	const EFPSCoreDeathState OldDeathState = DeathState;
	
	DeathState = NewDeathState;
	
	HandleDeathStateChanged(OldDeathState);
}

void UFPSCoreHealthComponent::OnRep_DeathState(EFPSCoreDeathState OldDeathState)
{
	HandleDeathStateChanged(OldDeathState);
}

void UFPSCoreHealthComponent::HandleDeathStateChanged(EFPSCoreDeathState OldDeathState)
{
	if (OldDeathState == EFPSCoreDeathState::NotDead && DeathState == EFPSCoreDeathState::DeathStarted)
	{
		OnDeathStarted.Broadcast(this);
		return;
	}
	if (OldDeathState == EFPSCoreDeathState::DeathStarted && DeathState == EFPSCoreDeathState::DeathFinished)
	{
		OnDeathFinished.Broadcast(this);
	}
}

void UFPSCoreHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UFPSCoreHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

