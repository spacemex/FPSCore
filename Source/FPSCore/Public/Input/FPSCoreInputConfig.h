// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "FPSCoreInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FPSCORE_API FFPSCoreInputAction
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Input")
	TObjectPtr<const UInputAction> InputAction = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Input",meta=(ExposeFunctionCategories="Input"))
	FGameplayTag InputTag;
};

USTRUCT(BlueprintType)
struct FPSCORE_API FFPSCoreInputMappingContext
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Input")
	TObjectPtr<const UInputMappingContext> InputMappingContext = nullptr;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Input")
	int32 Priority = 0;
};


UCLASS(BlueprintType,Const)
class FPSCORE_API UFPSCoreInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Input|Mapping Contexts")
	TArray<FFPSCoreInputMappingContext> MappingContexts;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Input|Ability Actions",meta=(TitleProperty=InputTag))
	TArray<FFPSCoreInputAction> NativeInputActions;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Input|Ability Actions")
	TArray<FFPSCoreInputAction> AbilityInputActions;
};