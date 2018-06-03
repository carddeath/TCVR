// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeypadWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UKeypadWidget : public UUserWidget
{
	GENERATED_BODY()

//Variables
public:

protected:

	//When this reaches 4 we must compare it to the code generated in the event script for the keypad
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int32 CurrentDigitInSequence = 0;

private:

//Methods

public:

	//Call this from the Keypad and compare it to the Current Digit in the sequence
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void InsertNewDigitIntoSystem(int32 DigitToEnter);

	//Called if the door didn't lock
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void SetAllDisplayToStarsEvent();

protected:

	UFUNCTION(BlueprintCallable, Category = "TextEdit")
		void ResetAllText(TArray<class UTextBlock*> Digits);

	UFUNCTION(BlueprintCallable, Category = "TextEdit")
		void SetAllDisplayToStars(TArray<UTextBlock*> Digits);

private:
	
	
	
	
};
