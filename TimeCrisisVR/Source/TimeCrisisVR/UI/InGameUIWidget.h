// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UInGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

//Variables
public:

protected:

private:
	
	//A counter before we stop the loop of the timer
	int32 AmtOfFlashesOnStart = 0;
	int32 AmtOfFlashesOfWait = 0;

	FTimerHandle FlashAreaStartHandle;
	FTimerHandle FlashWaitHandle;
	
//Methods
public:

	//Is part of a delegate set to do this in CPP. We call this event to set the text, callable then does the time start and StartTimer does the flashing
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void FlashUpAreaStart(int32 AreaNum);
	


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
		void HideWait();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void DisplayAction();

protected:

	UFUNCTION(BlueprintCallable, Category = "Callable Functions")
		void FlashUpAreaStartCallable(class UTextBlock* AreaStart);

	//Can be called via code as a delegate but also in BP via the flash up start
	UFUNCTION(BlueprintCallable, Category = "Events")
		void FlashUpWait(UTextBlock* WaitTB);

private:

	void FlashUpAreaStartTimer(UTextBlock* AreaStart);

	void FlashUpWaitStartTimer(UTextBlock* WaitTB);

	
	
};
