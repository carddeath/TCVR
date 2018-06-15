// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndOfTrialTimer.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UEndOfTrialTimer : public UUserWidget
{
	GENERATED_BODY()

		//Variables

protected:

	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool bCountTimerDown = false;


	//Methods
public:
	void SetCountTimerDown();

protected:
	UFUNCTION(BlueprintCallable, Category = "EndGame")
		void GenerateNewLocomotionModifier();

private:



	//Loads the level again
	void LoadLevelAgain();


	
	
	
};
