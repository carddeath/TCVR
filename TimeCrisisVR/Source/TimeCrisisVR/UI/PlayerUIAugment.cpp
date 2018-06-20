// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUIAugment.h"
#include "Components/WidgetComponent.h"
#include "Managers/NavigationManager.h"
#include "Managers/EventManager.h"
#include "Public/EngineUtils.h"
#include "UI/InGameUIWidget.h"
#include "UI/EndOfAreaDisplay.h"
#include "Managers/DataTracker.h"
#include "UI/EndOfTrialTimer.h"
#include "Player/VRPawn.h"

// Sets default values
APlayerUIAugment::APlayerUIAugment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WidgetDisplay = CreateDefaultSubobject<UWidgetComponent>(FName("Widget Display"));
}

// Called when the game starts or when spawned
void APlayerUIAugment::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing The world in Augment"))
			return;
	}

	for (TActorIterator<AEventManager> ita(GetWorld()); ita; ++ita) 
	{
		EventManager = *ita;
	}

	for (TActorIterator<ANavigationManager> ita(GetWorld()); ita; ++ita) 
	{
		NavManager = *ita;
	}

	for (TActorIterator<ADataTracker> ita(GetWorld()); ita; ++ita)
	{
		DataTracker = *ita;
	}

	//Creates InGameUIWidget and sets it
	InGameUiWidget = CreateWidget<UInGameUIWidget>(GetWorld(), InGameUITemplate);
	WidgetDisplay->SetWidget(InGameUiWidget);

	//Creates the end of game widget to be assigned later
	//EndOfAreaWidget = CreateWidget<UEndOfAreaDisplay>(GetWorld(), EndOfAreaTemplate);

	//Creates the trial end widget
	//EndOfTrialWidget = CreateWidget<UEndOfTrialTimer>(GetWorld(), EndOfTrialTimerTemplate);

	//Assign all delegates

	if (NavManager) 
	{
		//A delegate that will show the area start if the section = 1 in the navigation manager
		NavManager->FlashUpDelegateAreaStart.AddDynamic(InGameUiWidget, &UInGameUIWidget::FlashUpAreaStart);
		//Need to call it myself here so that we have assigned the delegate
		NavManager->BroadCastStageArea();

		//Assign all other required delegates
		//Display Proceed when all enemies are dead
		NavManager->ToggleProceedDisplayDelegate.AddDynamic(InGameUiWidget, &UInGameUIWidget::DisplayProceed);

		//Remove Wait once the player arrives at a location and show ACTION
		NavManager->ToggleWaitOffDelegate.AddDynamic(InGameUiWidget, &UInGameUIWidget::DisplayWait);

		//Shows the end of the game data screen
		NavManager->ShowEndOfAreaWidgetDelegate.AddDynamic(this, &APlayerUIAugment::SwapWidgetsInGame);
	}

	if (EventManager) 
	{
		//We want to display reload and it comes from the event manager via the players gun
		EventManager->ReloadDisplayDelegate.AddDynamic(InGameUiWidget, &UInGameUIWidget::DisplayReload);
	}
		
	if (DataTracker) 
	{
		//Is used to retrieve the end of screen data
		DataTracker->DataSendingDelegate.AddDynamic(this, &APlayerUIAugment::SendEndOfAreaDataToWidget);
	}
}

// Called every frame
void APlayerUIAugment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerUIAugment::SwapWidgetsInGame(bool bShowEndOfAreaWidget)
{
	if (bShowEndOfAreaWidget) 
	{
		//FOR THE GAME, NOT THE EXPERIMENT
		//if (EndOfAreaWidget) 
		//{
		//	WidgetDisplay->SetWidget(EndOfAreaWidget);
		//	EndOfAreaWidget->DisplayEndOfGameScreenInOrder();
		//}

		if (EndOfTrialWidget) 
		{
			WidgetDisplay->SetWidget(EndOfTrialWidget);
			EndOfTrialWidget->SetCountTimerDown(0);
		}
	}
	else 
	{
		WidgetDisplay->SetWidget(InGameUiWidget);
	}
}

void APlayerUIAugment::SendEndOfAreaDataToWidget(FGameData GameData) 
{

	//EXPERIMENT ONLY
	EndOfTrialWidget = CreateWidget<UEndOfTrialTimer>(GetWorld(), EndOfTrialTimerTemplate);
	if (EndOfTrialWidget)
	{


		InGameUiWidget->RemoveFromViewport();
		WidgetDisplay->SetWidget(EndOfTrialWidget);
		
		//Passes the total time taken in the game
		EndOfTrialWidget->SetCountTimerDown(GameData.TotalTimeTaken);
	}

	//GAME ONLY
	//EndOfAreaWidget = CreateWidget<UEndOfAreaDisplay>(GetWorld(), EndOfAreaTemplate);
	/*if (EndOfAreaWidget) 
	{
		WidgetDisplay->SetWidget(EndOfAreaWidget);
		EndOfAreaWidget->GenerateAllDataToDisplay(GameData);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Missing End of area widget when sending data"));
	}*/

}

