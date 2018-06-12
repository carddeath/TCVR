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
#include "Tutorial/TutorialShootingTarget.h"
#include "Managers/EnemySpawner.h"
#include "Gameplay/NavigationArrow.h"

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

	//Get the enemy spawner so we can assign the delegate when all enemies are dead
	for (TActorIterator<AEnemySpawner> ita(GetWorld()); ita; ++ita)
	{
		EnemySpawner = *ita;
	}

	//Move the tutorial on when the enemies are all dead
	if (EnemySpawner) 
	{
		EnemySpawner->TutorialKilledEnemiesDele.AddDynamic(this, &ATutorialManager::ProceedTutorialStep);
		EnemySpawner->bIsTutorial = true;
	}

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
	TutorialMessages.Insert("Good job if you hit the target, if not you'll just keep trying in the experiment. If you look down now you'll notice some ammo pouches where you can ammo. Place your empty hand into the pouch and hold the grab button. Raise the ammo clip towards your gun to reload!"
		, 5);
	TutorialMessages.Insert("You now know how to reload your gun. See those enemy soldiers? They are your real targets. Once all enemies in an area are dead you'll see an arrow. Shoot it to proceed through the level! Kill all of the soldiers to proceed!"
		, 6);
	TutorialMessages.Insert("Well done. Now shoot the arrow to proceed to the experiment. If you need to take a break, remove the headset and speak to the experiment lead. Good luck!"
		, 7);
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
		PlayerCharacter->bTutorialBtnEnabled = false;

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
		PlayerCharacter->bTutorialBtnEnabled = false;

		for (auto& Box : HandSelectionBoxes)
		{
			Box->Destroy();
		}

		//Attempt to get the gun and set the value to true that we're in a tutorial
		PlayersGun = PlayerCharacter->GetPlayersGun();
		if (PlayersGun) 
		{
			PlayersGun->bTutorialHandSwap = true;
			PlayersGun->bTutorialEnabled = true;
		}
	}

	//Shooting the gun stage of the tutorial
	if (TutorialStepCounter == 4) 
	{
		PlayerCharacter->bTutorialBtnEnabled = false;

		if (PlayersGun) 
		{
			PlayersGun->bTutorialHandSwap = false;
			//Assign the delegate so that when we shoot the target we call this function and increment the number
			PlayersGun->TutorialTargetShotDelegate.AddDynamic(this, &ATutorialManager::ProceedTutorialStep);
		}

		//Create the target to be shot
		if (TargetTemplate) 
		{
			//Spawns a target in the middle of the room
			CurrentTarget = GetWorld()->SpawnActor<ATutorialShootingTarget>(TargetTemplate);
			CurrentTarget->SetActorLocation(FVector(310.0f, -670.0f, 100.0f));
			CurrentTarget->SetActorRotation(FRotator(90.0f, 0.0f, 0.0f));
		}
	}

	if (TutorialStepCounter == 5) 
	{
		PlayerCharacter->bTutorialBtnEnabled = false;
		CurrentTarget->Destroy();

		//Make the ammo pouch
		PlayerCharacter->CreateAmmoPouch();

		//We need the player to reload the gun and to proceed when it's done
		PlayersGun->TutorialReloadedGun.AddDynamic(this, &ATutorialManager::ProceedTutorialStep);
	}

	if (TutorialStepCounter == 6) 
	{
		//TODO: Create an enemy model in the scene and allow it to be shot, the player must kill it to proceed
		EnemySpawner->SpawnTutorialEnemies();
	}

	if (TutorialStepCounter == 7) 
	{
		if (EndLevelArrow) 
		{
			EndLevelArrow->ShowArrow(true);
		}
	}

	TutorialStepCounter++;

}

