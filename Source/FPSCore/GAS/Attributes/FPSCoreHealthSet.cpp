// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCoreHealthSet.h"

#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


UFPSCoreHealthSet::UFPSCoreHealthSet()
{
	InitMaxHealth(100.f);
	InitHealth(100.f);
}

void UFPSCoreHealthSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UFPSCoreHealthSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UFPSCoreHealthSet, MaxHealth, COND_None, REPNOTIFY_Always);
}

void UFPSCoreHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}else if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
}

void UFPSCoreHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute() && GetHealth() > NewValue)
	{
		SetHealth(NewValue);
	}
}

void UFPSCoreHealthSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	{
		SetMaxHealth(FMath::Max(Data.EvaluatedData.Magnitude, 1.f));
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
}

void UFPSCoreHealthSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFPSCoreHealthSet,Health,OldHealth);
}

void UFPSCoreHealthSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UFPSCoreHealthSet,MaxHealth,OldMaxHealth);
}