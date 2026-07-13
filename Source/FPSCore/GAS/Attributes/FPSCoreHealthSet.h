// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "FPSCoreHealthSet.generated.h"

#define FPSCORE_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class FPSCORE_API UFPSCoreHealthSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UFPSCoreHealthSet();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_Health, Category="FPS Core|Health")
	FGameplayAttributeData Health;
	
	FPSCORE_ATTRIBUTE_ACCESSORS(UFPSCoreHealthSet, Health);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing= OnRep_MaxHealth, Category="FPS Core|Health")
	FGameplayAttributeData MaxHealth;
	
	FPSCORE_ATTRIBUTE_ACCESSORS(UFPSCoreHealthSet, MaxHealth);
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	float GetHealthValue() const
	{
		return GetHealth();
	}
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	float GetMaxHealthValue() const
	{
		return GetMaxHealth();
	}
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	float GetHealthNormalized() const
	{
		const float CurrentMaxHealth = GetMaxHealth();
		return CurrentMaxHealth > 0.0f ? GetHealth() / CurrentMaxHealth : 0.0f;
	}
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	bool IsOutOfHealth() const
	{
		return GetHealth() <= 0.0f;
	}
	
protected:
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	
#undef FPSCORE_ATTRIBUTE_ACCESSORS

};
