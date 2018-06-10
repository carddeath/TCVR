// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialManager.h"

#include "Components/WidgetComponent.h"
#include "EngineUtils.h"

//Game Classes
#include "Player/VRPawn.h"

// Sets default values
ATutorialManager::ATutorialManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TutorialWidget = CreateDefaultSubobject<UWidgetComponent>(FName("Tutorial Widget Comp"));
}

// Called when the game starts or when spawned
void ATutorialManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Get the player character
	for (TActorIterator<AVRPawn> ita(GetWorld()); ita; ++ita) 
	{
		PlayerCharacter = *ita;
	}

	AssignDelegates();
}

// Called every frame
void ATutorialManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATutorialManager::AssignDelegates() 
{
	//Allows the tutorial to be ticked over by the VR pawn
	PlayerCharacter->TutorialProceedDelegate.AddDynamic(this, &ATutorialManager::ProceedTutorialStep);
}

void ATutorialManager::ProceedTutorialStep(int junk) 
{
	//TODO: Proceed over the tutorial step to the next page loading the correct text and image thats required
	UE_LOG(LogTemp, Warning, TEXT("Proceed Tutorial"));
}

