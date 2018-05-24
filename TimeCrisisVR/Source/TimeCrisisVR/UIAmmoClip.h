// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIAmmoClip.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UUIAmmoClip : public UUserWidget
{
	GENERATED_BODY()

	//Variables
public:

protected:

private:


	//Methods
public:
	virtual void Construct();


	UFUNCTION(BlueprintImplementableEvent, Category = "External Call")
	void UpdateDisplayRemoteCall(bool bWasReload, int32 AmmoQuantity);

protected:

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void UpdateDisplay(class UTextBlock* AmmoTextUI, class UVerticalBox* BulletVBBox, bool bWasReload, int32 AmmoQuantity);

private:

	
	
	
};
