// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Timer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEndOfGameTrigger, int, junk);

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UTimer : public UUserWidget
{
	GENERATED_BODY()

	//Variables
public:

	FEndOfGameTrigger EndOfGameTriggerDelegate;

protected:

	//The time currently on the clock
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timing Logic")
	float TimeRemaining = 60.0f;

	//The time currently on the clock
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Timing Logic")
		float TimeUntilDanger = 10.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Timing Logic")
		bool bIsTimerPaused = false;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* TimeAddedSFX = nullptr;
private:

	//Methods
public:
	
	//Add time after each stage
	void AddTimeToTotal(float AmtToAdd);

	void TogglePauseOnTimer(bool bShouldPause);

protected:

	UFUNCTION(BlueprintCallable, Category = "EndOfGameEvent")
		void WasEndOfGame();

private:
		
};
