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
	int32 AmtOfFlashesOnWait = 0;

	FTimerHandle FlashAreaStartHandle;
	FTimerHandle FlashWaitHandle;
	
//Methods
public:

	//Is part of a delegate set to do this in CPP. We call this event to set the text, callable then does the time start and StartTimer does the flashing
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void FlashUpAreaStart(int32 AreaNum);

	
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void DisplayProceed(bool bShouldDisplayProceed);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void DisplayWait(bool bShouldDisplayWait);

	//leaving action as an event due to the fact it will always disappear
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Events")
		void DisplayAction();

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void DisplayReload(bool bShouldDisplayWait);

protected:

	UFUNCTION(BlueprintCallable, Category = "Callable Functions")
		void FlashUpAreaStartCallable(class UTextBlock* AreaStart);

	UFUNCTION(BlueprintCallable, Category = "Callable Functions")
		void DisplayWaitCallable(bool bShouldDisplayWait, UTextBlock* WaitTB);

	UFUNCTION(BlueprintCallable, Category = "Callable Functions")
		void DisplayProceedCallable(bool bShouldDisplayProceed, UTextBlock* ProceedTB);

	UFUNCTION(BlueprintImplementableEvent, Category = "Callable Functions")
		void HideWait();


private:

	void FlashUpAreaStartTimer(UTextBlock* AreaStart);
	
};
