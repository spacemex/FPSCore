// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FPSCoreHealthComponent.generated.h"

class UFPSCoreAbilitySystemComponent;
class UFPSCoreHealthSet;
struct FOnAttributeChangeData;

UENUM(BlueprintType)
enum class EFPSCoreDeathState : uint8
{
	NotDead,
	DeathStarted,
	DeathFinished,
};

class UFPSCoreHealthComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FFPSCoreHealthChangedSignature,UFPSCoreHealthComponent*, HealthComponent,
	float, OldValue, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFPSCoreDeathEventSignature, UFPSCoreHealthComponent*, HealthComponent);


UCLASS(BlueprintType,Blueprintable,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FPSCORE_API UFPSCoreHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFPSCoreHealthComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	void InitializeWithAbilitySystem(UFPSCoreAbilitySystemComponent* InAbilitySystemComponent, UFPSCoreHealthSet* InHealthSet);
	void UninitializeFromAbilitySystem();
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	UFPSCoreAbilitySystemComponent* GetAbilitySystemComponent() const
	{
		return AbilitySystemComponent;
	}
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	UFPSCoreHealthSet* GetHealthSet() const
	{
		return HealthSet;
	}
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	float GetHealth() const;
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	float GetHealthNormalized() const;
	UFUNCTION(BlueprintPure,Category="FPS Core|Healt")
	EFPSCoreDeathState GetDeathState() const
	{
		return DeathState;
	}
	UFUNCTION(BlueprintPure,Category="FPS Core|Health")
	bool IsDead() const
	{
		return DeathState != EFPSCoreDeathState::NotDead;
	}
	UFUNCTION(BlueprintCallable,Category="FPS Core|Health")
	void FinishDeath();
	
	UPROPERTY(BlueprintAssignable,Category="FPS Core|Health")
	FFPSCoreHealthChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable,Category="FPS Core|Health")
	FFPSCoreHealthChangedSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable,Category="FPS Core|Health")
	FFPSCoreDeathEventSignature OnDeathStarted;
	UPROPERTY(BlueprintAssignable,Category="FPS Core|Health")
	FFPSCoreDeathEventSignature OnDeathFinished;
	
private:
	void HandHealthChanged(const FOnAttributeChangeData& ChangeData);
	void HandMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
	void StartDeath();
	void SetDeathState(EFPSCoreDeathState NewDeathState);
	void HandleDeathStateChanged(EFPSCoreDeathState OldDeathState);
	
	UFUNCTION()
	void OnRep_DeathState(EFPSCoreDeathState OldDeathState);
	
	UPROPERTY(Transient)
	TObjectPtr<UFPSCoreAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY(Transient)
	TObjectPtr<UFPSCoreHealthSet> HealthSet;

	UPROPERTY(ReplicatedUsing = OnRep_DeathState,Transient)EFPSCoreDeathState DeathState = EFPSCoreDeathState::NotDead;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
