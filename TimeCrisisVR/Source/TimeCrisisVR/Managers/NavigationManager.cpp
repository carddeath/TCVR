// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigationManager.h"

#include "Managers/EnemySpawner.h"
#include "Engine.h"
#include "Player/MainPlayerController.h"
#include "Gameplay/NavigationArrow.h"
#include "Managers/EventManager.h"
#include "Managers/TImeManager.h"

// Sets default values
ANavigationManager::ANavigationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANavigationManager::BeginPlay()
{
	Super::BeginPlay();

	//Get the enemy spawner
	for (TActorIterator<AEnemySpawner> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		EnemySpawner = *ActorIt;
		if (!EnemySpawner)
		{
			UE_LOG(LogTemp, Error, TEXT("Missing Enemy Spawner when searching in %s"), *this->GetName());
		}
	}

	//Get the navigation manager
	for (TActorIterator<AEventManager> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		EventManager = *ActorIt;
		if (!EventManager)
		{
			UE_LOG(LogTemp, Error, TEXT("Missing Event manager when searching in %s"), *this->GetName());
		}
	}

	//Get the time manager
	for (TActorIterator<ATImeManager> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		TimeManager = *ActorIt;
		if (!TimeManager)
		{
			UE_LOG(LogTemp, Error, TEXT("Missing Timer when searching in %s"), *this->GetName());
		}
	}

	EnemySpawner->RevealNextPointToNavMang.AddDynamic(this, &ANavigationManager::RevealNextLocomotionArrow);

	//Save a reference to the player controller
	CustomPlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	//Disable all arrows
	for (auto& Arrow : LocomotionPoints) 
	{
		Arrow->ShowArrow(false);
	}

	//Assign the current section to the enemy spawner so we have the right info for debugging
	CurrentSection = EnemySpawner->CurrentSection;
}

// Called every frame
void ANavigationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//When the player is moving we check to see if they've arrived at their destination. If so then we stop checking and change the UI and shout action!
	if (bHasPlayerSetOffToNextPoint) 
	{
		float XResult = FMath::Abs(CustomPlayerController->GetPawn()->GetActorLocation().X - LocomotionPoints[CurrentSection-1]->GetActorLocation().X);
		float YResult = FMath::Abs(CustomPlayerController->GetPawn()->GetActorLocation().Y - LocomotionPoints[CurrentSection - 1]->GetActorLocation().Y);

		if (XResult <= 0.5f && YResult <= 0.5f)
		{
			bHasPlayerSetOffToNextPoint = false;

			// Remove wait and show action
			if (ToggleWaitOffDelegate.IsBound()) 
			{
				ToggleWaitOffDelegate.Broadcast(false);
				//EventManager->AnnouncerCallThroughAction();
			}

			if (TurnGunToEnabled.IsBound()) 
			{
				TurnGunToEnabled.Broadcast(0);
			}

			//TODO: Add a delay based on when the player is in the correct position
			EnemySpawner->UpdateSection(CurrentSection);
		}
	}
}

void ANavigationManager::UpdateCurrentSection(bool bTeleportPlayer) 
{
	//We dont increment at first as the current section is one value ahead of the 0 array of locomotion points

	if (LocomotionPoints.Num() <= 0 || CurrentSection >= LocomotionPoints.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Locomotion points on %s"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Missing locomotion points on Navigation Manager or too few sub stage locomotion points"));
		return;
	}

	//Do the locomotion and send the location of the next point
	if (CustomPlayerController) 
	{
		//Used to decide on which method of teleportation should be used.
		if (!bTeleportPlayer) 
		{
			CustomPlayerController->MovePlayerViaNavManagerNodeBased(LocomotionPoints[CurrentSection]->GetActorLocation(), LocomotionPoints[CurrentSection]->GetActorRotation());
		}
		else 
		{
			CustomPlayerController->MovePlayerViaNavManagerTeleport(LocomotionPoints[CurrentSection]->GetActorLocation(), LocomotionPoints[CurrentSection]->GetActorRotation());
		}
	}

	//Hide the arrow that was just shot, is no longer needed
	LocomotionPoints[CurrentSection]->ShowArrow(false);

	//Increment the current stage
	CurrentSection++;

	//Delay added, now found in the tick component
	//EnemySpawner->UpdateSection(CurrentSection);

	//Check to see if there are events that should happen
	EventChecker();

	//Add time to the players limit and resume the timer
	TimeManager->PauseOrResumeTimer(false);
	TimeManager->AddTimeToTimersForSectionBasedOnArea(CurrentStage, CurrentArea);

	//Lets Hide proceed on the UI and show Wait as we are travelling
	if (ToggleProceedDisplayDelegate.IsBound())
	{
		ToggleProceedDisplayDelegate.Broadcast(false);
		bHasPlayerSetOffToNextPoint = true;
	}
}

void ANavigationManager::RevealNextLocomotionArrow(int junk) 
{
	//We check + 1 as we have no currently updated the current sub stage so we need to check if there is another point beyond this one
	if (LocomotionPoints.Num() <= 0 || CurrentSection >= LocomotionPoints.Num()) 
	{
		//We have ran out of points, TODO: Launch a delegate to show the end of game data
		UE_LOG(LogTemp, Error, TEXT("End of locomotion points on %s"), *this->GetName());

		//Lets get the time after pausing at the end of an area/game
		TimeManager->PauseOrResumeTimer(true);

		//Let's allow it to collect all data for the display
		if (AllowDataCollectionFromClassesDelegate.IsBound())
		{
			AllowDataCollectionFromClassesDelegate.Broadcast(0);
		}
		return;
	}
		LocomotionPoints[CurrentSection]->ShowArrow(true);
		TimeManager->PauseOrResumeTimer(true);

		//We want to display the text of Proceed so the player knows to shoot the next arrow as all enemies have been killed. Send true as we want to display
		if (ToggleProceedDisplayDelegate.IsBound())
		{
			ToggleProceedDisplayDelegate.Broadcast(true);
		}
}

void ANavigationManager::EventChecker() 
{
	//If we're by the hanger door lets start the timer
	if (CurrentStage == 1 && CurrentArea == 1 && CurrentSection == 3 && EventManager) 
	{
		EventManager->StartTimerOnHangerDoorToClose();
	}
	//If we shoot the arrow before the timer is up. As we shot the arrow we will be 1 section ahead so we subtract 1 from the current.
	else if (CurrentStage == 1 && CurrentArea == 1 && CurrentSection - 1 == 3 && EventManager) 
	{
		//If the door didn't close then stop the timer
		if (!EventManager->bDoorBeganToClose) 
		{
			EventManager->StopTimerOnHangerDoor();
		}
	}
}

//Have to put this here to prevent the begin plays being called in the wrong order
void ANavigationManager::BroadCastStageArea() 
{
	//We do a check for the stage 1 delegate to broadcoast 
	if (CurrentSection == 1)
	{

		if (FlashUpDelegateAreaStart.IsBound())
		{
			FlashUpDelegateAreaStart.Broadcast(CurrentArea);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to launch UI delegate in %s"), *this->GetName());
		}

	}
}

//Getters

int32 ANavigationManager::GetCurrentGameStage() 
{
	return CurrentStage;
}

int32 ANavigationManager::GetCurrentGameArea()
{
	return CurrentArea;
}

int32 ANavigationManager::GetCurrentGameSection()
{
	return CurrentSection;
}

