// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures/GameData.h"

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

	//The main UI widget that we use during gameplay
	UPROPERTY(EditDefaultsOnly, Category = "Widget Subclasses")
		TSubclassOf<class UInGameUIWidget> InGameUITemplate;

	//The UI widget we assign when we reach the end of the game
	UPROPERTY(EditDefaultsOnly, Category = "Widget Subclasses")
		TSubclassOf<class UEndOfAreaDisplay> EndOfAreaTemplate;

	//Experiment ONLY: Used to show the timer counting down at the end
	UPROPERTY(EditDefaultsOnly, Category = "Widget Subclasses")
		TSubclassOf<class UEndOfTrialTimer> EndOfTrialTimerTemplate;

private:

	class ANavigationManager* NavManager = nullptr;

	class AEventManager* EventManager = nullptr;

	class UInGameUIWidget* InGameUiWidget = nullptr;

	class UEndOfAreaDisplay* EndOfAreaWidget = nullptr;

	UEndOfTrialTimer* EndOfTrialWidget = nullptr;

	class ADataTracker* DataTracker = nullptr;

	//Methods
	
public:	
	APlayerUIAugment();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	

	UFUNCTION()
		void SwapWidgetsInGame(bool bShowEndOfAreaWidget);

	UFUNCTION()
		void SendEndOfAreaDataToWidget(FGameData GameData);
	
};
