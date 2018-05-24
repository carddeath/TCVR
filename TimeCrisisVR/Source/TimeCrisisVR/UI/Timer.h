// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Timer.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UTimer : public UUserWidget
{
	GENERATED_BODY()

	//Variables
public:

protected:

	//The time currently on the clock
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timing Logic")
	float TimeRemaining = 60.0f;

	//The time currently on the clock
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing Logic")
		float TimeUntilDanger = 10.0f;

private:

	//Methods
public:
	
	//Add time after each stage
	void AddTimeToTotal(float AmtToAdd);

protected:


private:
	
	
	
};
