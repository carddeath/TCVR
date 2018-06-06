// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUIAugment.h"
#include "Components/WidgetComponent.h"
#include "Managers/NavigationManager.h"
#include "Managers/EventManager.h"
#include "Public/EngineUtils.h"
#include "UI/InGameUIWidget.h"
#include "UI/EndOfAreaDisplay.h"
#include "Managers/DataTracker.h"

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
	EndOfAreaWidget = CreateWidget<UEndOfAreaDisplay>(GetWorld(), EndOfAreaTemplate);

	//Assign all delegates
	if (InGameUiWidget && NavManager && EventManager && EndOfAreaWidget && DataTracker && InGameUITemplate && EndOfAreaTemplate)
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

		//We want to display reload and it comes from the event manager via the players gun
		EventManager->ReloadDisplayDelegate.AddDynamic(InGameUiWidget, &UInGameUIWidget::DisplayReload);

		NavManager->ShowEndOfAreaWidgetDelegate.AddDynamic(this, &APlayerUIAugment::SwapWidgetsInGame);

		DataTracker->DataSendingDelegate.AddDynamic(this, &APlayerUIAugment::SendEndOfAreaDataToWidget);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Something on %s"), *this->GetName());
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
		WidgetDisplay->SetWidget(EndOfAreaWidget);
		if (EndOfAreaWidget) 
		{
			EndOfAreaWidget->DisplayEndOfGameScreenInOrder();
		}
	}
	else 
	{
		WidgetDisplay->SetWidget(InGameUiWidget);
	}
}

void APlayerUIAugment::SendEndOfAreaDataToWidget(FGameData GameData) 
{
	//CAREFUL: HOPEFULLY NOT A RACE CONDITION WITH THE INFOMATION ABOVE
	EndOfAreaWidget->GenerateAllDataToDisplay(GameData);
}

