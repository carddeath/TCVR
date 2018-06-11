// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialManager.h"

#include "Components/WidgetComponent.h"
#include "EngineUtils.h"
#include "Engine/Texture2D.h"

//Game Classes
#include "Player/VRPawn.h"
#include "Tutorial/TutorialWidget.h"
#include "Tutorial/HandSelectionBox.h"
#include "Player/PlayersGun.h"

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

	PlayerCharacter->bIsTutorial = true;

	GenerateTutorialMessageScreens();
	AssignDelegates();
	HideAllObjects();

	//Call this once to get the first step to show
	ProceedTutorialStep(0);
}

// Called every frame
void ATutorialManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATutorialManager::GenerateTutorialMessageScreens() 
{
	TutorialMessages.Insert("Welcome to the experiment. This level aims to get you familiar with the environment and controls that you'll be using for around 10 minutes. Rotate your head and look up and down to get a feel for the movement.To proceed press A or X."
		, 0);
	TutorialMessages.Insert("Notice the boxes to your left and right. In the experiment you'll aim to avoid being hit as much as possible by enemies. Learn to peek out of cover to shoot and to hide when bullets are coming towards you. To proceed press A or X."
		, 1);
	TutorialMessages.Insert("Please tell us, are you left or right handed? Just stare at the box stated 'Left' or 'Right' depending on your preference!"
		, 2);
	TutorialMessages.Insert("You can swap hands at anytime as long as the other hand is empty! Just point your empty hand towards the hand holding the gun. Press the 'Grip' button to take the gun. If you're having trouble move your hands closer together!"
		, 3);
	TutorialMessages.Insert("Now that you know how to change hands let's look at firing your weapon. Point the gun at the target and press the trigger on the hand holding the gun to fire a shot. Notice how you only have 6 bullets."
		, 4);
}

void ATutorialManager::AssignDelegates() 
{
	//Allows the tutorial to be ticked over by the VR pawn
	PlayerCharacter->TutorialProceedDelegate.AddDynamic(this, &ATutorialManager::ProceedTutorialStep);
	PlayerCharacter->BeginBoxStareAnimation.AddDynamic(this, &ATutorialManager::GetHandSelectionActor);
}

void ATutorialManager::HideAllObjects() 
{
	for(auto& Box : HandSelectionBoxes)
	{
		Box->ShowBox(false);
	}
}

void ATutorialManager::GetHandSelectionActor(AActor* BoxToHighlight, bool bStartAnim) 
{
	if (bStartAnim) 
	{
		Cast<AHandSelectionBox>(BoxToHighlight)->ShowTheBoxFilling();
	}
	else 
	{
		Cast<AHandSelectionBox>(BoxToHighlight)->ResetBoxPosition();
	}
}

void ATutorialManager::ProceedTutorialStep(int junk) 
{
	//TODO: Proceed over the tutorial step to the next page loading the correct text and image thats required
	UE_LOG(LogTemp, Warning, TEXT("Proceed Tutorial"));

	//Replace nullptr with the array, make sure it's the right size = to the max amount of steps
	Cast<UTutorialWidget>(TutorialWidget->GetUserWidgetObject())->UpdateVisualsInTutorial(TutorialMessages[TutorialStepCounter], nullptr);

	//Picking if you're left or right handed with a pistol
	if (TutorialStepCounter == 2) 
	{
		//We need to start looking with the line trace for where we are focusing
		PlayerCharacter->bTutorialIsSearchingForHands = true;

		for (auto& Box : HandSelectionBoxes)
		{
			Box->ShowBox(true);
		}
	}


	//Letting the player know how to swap hands with the gun
	if (TutorialStepCounter == 3) 
	{
		for (auto& Box : HandSelectionBoxes)
		{
			Box->ShowBox(false);
		}

		//Attempt to get the gun and set the value to true that we're in a tutorial
		PlayersGun = PlayerCharacter->GetPlayersGun();
		if (PlayersGun) 
		{
			PlayersGun->bTutorialHandSwap = true;
		}
	}

	//Shooting the gun stage of the tutorial
	if (TutorialStepCounter == 4) 
	{
		if (PlayersGun) 
		{
			PlayersGun->bTutorialHandSwap = false;
		}
	}

	TutorialStepCounter++;

}

