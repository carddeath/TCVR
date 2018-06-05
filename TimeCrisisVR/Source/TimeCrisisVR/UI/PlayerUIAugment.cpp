// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerUIAugment.h"
#include "Components/WidgetComponent.h"
#include "Managers/NavigationManager.h"
#include "Public/EngineUtils.h"
#include "UI/InGameUIWidget.h"

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

	for (TActorIterator<ANavigationManager> ita(GetWorld()); ita; ++ita) 
	{
		NavManager = *ita;

		if (!NavManager) 
		{
			UE_LOG(LogTemp, Error, TEXT("Missing Navigation Manager on %s"), *this->GetName());
		}
	}

	InGameUiWidget = Cast<UInGameUIWidget>(WidgetDisplay->GetUserWidgetObject());

	if (InGameUiWidget)
	{
		//A delegate that will show the area start if the section = 1 in the navigation manager
		NavManager->FlashUpDelegateAreaStart.AddDynamic(InGameUiWidget, &UInGameUIWidget::FlashUpAreaStart);
		//Need to call it myself here so that we have assigned the delegate
		NavManager->BroadCastStageArea();
	}
}

// Called every frame
void APlayerUIAugment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

