// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TimeCrisisVRGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API ATimeCrisisVRGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Reset State")
		void LoadNewLevel();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Reset State")
		void LoadLocomotionMainLevel();
	
	
};
