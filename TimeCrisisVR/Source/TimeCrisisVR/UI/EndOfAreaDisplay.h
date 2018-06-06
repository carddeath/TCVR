// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures/GameData.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndOfAreaDisplay.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UEndOfAreaDisplay : public UUserWidget
{
	GENERATED_BODY()

	//Variables
public:
	
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Data")
		FGameData GameData;

private:



	//Methods
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
		void GenerateAllDataToDisplay(FGameData GD);

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void DisplayEndOfGameScreenInOrder();

protected:

private:
	
	
	
};
