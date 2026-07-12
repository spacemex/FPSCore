// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSCorePlayerController.generated.h"

class UFPSCoreInputConfig;

UCLASS()
class FPSCORE_API AFPSCorePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
	
	UFUNCTION(BlueprintPure,Category="FPS Core|Input")
	const UFPSCoreInputConfig* GetInputConfig() const {return InputConfig;}
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="FPS Core|Input")
	TObjectPtr<UFPSCoreInputConfig> InputConfig;
	
private:
	void ApplyInputMappings();
};
