// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerUIAugment.generated.h"



UCLASS()
class TIMECRISISVR_API APlayerUIAugment : public AActor
{
	GENERATED_BODY()

	//Variables
public:



protected:

	UPROPERTY(EditDefaultsOnly, Category = "WidgetComponent")
		class UWidgetComponent* WidgetDisplay = nullptr;

private:

	class ANavigationManager* NavManager = nullptr;

	class UInGameUIWidget* InGameUiWidget = nullptr;

	//Methods
	
public:	
	APlayerUIAugment();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	
	
};
