// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()

	//Variables
public:

protected:

private:

	//Methods
public:

	//used to update the widget
	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void UpdateVisualsInTutorial(const FString& TextToDisplay, class UTexture2D* ImgToDisplay);

protected:

private:
	
	
	
	
};
