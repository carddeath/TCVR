// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigationManager.h"

#include "Managers/EnemySpawner.h"
#include "Engine.h"
#include "Player/MainPlayerController.h"
#include "Gameplay/NavigationArrow.h"
#include "Managers/EventManager.h"
#include "Managers/TImeManager.h"
#include "Gameplay/PlayerConstraintArea.h"


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

	//Wire up this delegate if the locomotion mode is teleporting
	CustomPlayerController->TeleportLocomodeDataBaseUpdate.AddDynamic(this, &ANavigationManager::UpdateCurrentSection);

	//Disable all arrows
	for (auto& Arrow : LocomotionPoints) 
	{
		Arrow->ShowArrow(false);
	}

	//Disable all pads
	for (auto& Pad : PointAndTPAreas)
	{
		Pad->ShowTeleportPad(false);
	}

	//Assign the current section to the enemy spawner so we have the right info for debugging
	CurrentSection = EnemySpawner->CurrentSection;

	if (GetWorld() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing The world"))
			return;
	}

	//Get the data instance
	DataInstance = Cast<UTutorialToGameSaveInstance>(GetWorld()->GetGameInstance());

	//DEBUG Just to change the type to quickly test

	//Do it normally from the data instance if we are not in debug
	if (!bIsInDebug) 
	{
		//Lets allow the arc to happen or not happen depending, will be checked in main player controller blueprint
		if (DataInstance->GetLocomotionType() == ELocomotionType::NODE_BASED)
		{
			CustomPlayerController->SetIfAllowedToTeleportViaArcForDuration(false);
		}
		else if (DataInstance->GetLocomotionType() == ELocomotionType::POINT_AND_TELEPORT)
		{
			CustomPlayerController->SetIfAllowedToTeleportViaArcForDuration(true);
		}
	}
	//Read the debug value and do the same instruction but based on the debug value
	else 
	{
		DataInstance->SetLocomotionType(LocoType);
		if (LocoType == ELocomotionType::NODE_BASED) 
		{
			CustomPlayerController->SetIfAllowedToTeleportViaArcForDuration(false);
		}
		else if (LocoType == ELocomotionType::POINT_AND_TELEPORT) 
		{
			CustomPlayerController->SetIfAllowedToTeleportViaArcForDuration(true);
		}
	}

	//Sets the locomotionModiferToUse
	SetLocomotionModifer();
}

// Called every frame
void ANavigationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//When the player is moving we check to see if they've arrived at their destination. If so then we stop checking and change the UI and shout action!
	if (bHasPlayerSetOffToNextPoint) 
	{
		//Need to calculate the different depending on if its the arrow the actual teleportation pad. Might even need different ranges for the actual pad as it's a larger object

		if (DataInstance->GetLocomotionType() == ELocomotionType::NODE_BASED) 
		{
			XResult = FMath::Abs(CustomPlayerController->GetPawn()->GetActorLocation().X - LocomotionPoints[CurrentSection - 1]->GetActorLocation().X);
			YResult = FMath::Abs(CustomPlayerController->GetPawn()->GetActorLocation().Y - LocomotionPoints[CurrentSection - 1]->GetActorLocation().Y);

			//normalise the position for fade check

			if (ModType == EModifierTypes::FADE) 
			{
				float PlayPos = CustomPlayerController->GetPawn()->GetActorLocation().X;
				float EndPoint = LocomotionPoints[CurrentSection - 1]->GetActorLocation().X;
				float StartPoint = LocomotionPoints[CurrentSection - 2]->GetActorLocation().X;
				float NoramlizeDifference = (PlayPos - StartPoint) / (EndPoint - StartPoint);

				if (NoramlizeDifference <= 0.1f && !bBeganFadeOnNode)
				{
					bBeganFadeOnNode = true;
					FadeOnNodeLocomotion(true);
				}
				else if (NoramlizeDifference >= 0.9f && bBeganFadeOnNode)
				{
					bBeganFadeOnNode = false;
					FadeOnNodeLocomotion(false);
				}
				UE_LOG(LogTemp, Error, TEXT("VectorNormalized: %f"), NoramlizeDifference);
			}

			//UE_LOG(LogTemp, Warning, TEXT("XResult is %f, and YResult is %f"), XResult, YResult);
			if (XResult <= 2.0f && YResult <= 2.0f)
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

				//If we are in node based and we are forced into a rotation then lets tell the customer manager to do so
				if (ModType == EModifierTypes::FORCED_ROTATION || ModType == EModifierTypes::ANNOTATED_ROTATION) 
				{
					CustomPlayerController->RotatePlayerAfterTeleport();
				}
			}
		}
	}
}

void ANavigationManager::SetLocomotionModifer() 
{
	//Generates the type of modifier to apply to the locomotion based on the index inside of the tutorial instance
	if (!DataInstance) 
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Data Instance on %s"), *this->GetName());
		return;
	}

	//If we are not in debug then we pick the modifier based on the trail otherwise load the modtype loaded
	if (!bIsInDebug) 
	{
		switch (DataInstance->GetCurrentTrail())
		{
		case 0:
			ModType = EModifierTypes::NONE;
			DataInstance->SetModifierType(ModType);
			UE_LOG(LogTemp, Error, TEXT("No modifer applied"));
			break;
		case 1:
			ModType = EModifierTypes::FADE;
			DataInstance->SetModifierType(ModType);
			UE_LOG(LogTemp, Error, TEXT("Fade applied"));
			break;
		case 2:
			ModType = EModifierTypes::FORCED_ROTATION;
			DataInstance->SetModifierType(ModType);
			UE_LOG(LogTemp, Error, TEXT("Forced Rotation applied"));
			break;
		case 3:
			ModType = EModifierTypes::ANNOTATED_ROTATION;
			DataInstance->SetModifierType(ModType);
			UE_LOG(LogTemp, Error, TEXT("Annotated Rotation applied"));
			break;
		default:
			break;
		}
	}
	else 
	{
		DataInstance->SetModifierType(ModType);
	}

	//Sets up the visuals of all the locomotion points to either an arrow mesh or an orb mesh depending on the locomotion type
	if (LocoType == ELocomotionType::NODE_BASED) 
	{
		for (auto& LocoNode : LocomotionPoints) 
		{
			if (ModType == EModifierTypes::ANNOTATED_ROTATION) 
			{
				LocoNode->SetArrowMeshes(true);
			}
			else 
			{
				LocoNode->SetArrowMeshes(false);
			}

		}
	}
	else 
	{
		//SHOW OR HIDE THE ARROW TEXTURE ON THE WALLS
	}

}

void ANavigationManager::UpdateCurrentSection() 
{
	//We dont increment at first as the current section is one value ahead of the 0 array of locomotion points

	//Doing all checks required for node based locomotion
	if (DataInstance->GetLocomotionType() == ELocomotionType::NODE_BASED) 
	{
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
			CustomPlayerController->SetLocationToMoveAndRotation(LocomotionPoints[CurrentSection]->GetActorLocation(), LocomotionPoints[CurrentSection]->GetActorRotation());
			CustomPlayerController->SetModiferState(ModType, DataInstance->GetLocomotionType());
			bHasPlayerSetOffToNextPoint = true;
		}

		//Hide the arrow that was just shot, is no longer needed
		LocomotionPoints[CurrentSection]->ShowArrow(false);
	}
	else if (DataInstance->GetLocomotionType() == ELocomotionType::POINT_AND_TELEPORT) 
	{
		if (PointAndTPAreas.Num() <= 0 || CurrentSection >= PointAndTPAreas.Num())
		{
			UE_LOG(LogTemp, Error, TEXT("Missing PointAndTPAreas on %s"), *this->GetName());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Missing PointAndTPAreas on Navigation Manager or too few sub stage PointAndTPAreas points"));
			return;
		}

		//Do the locomotion and send the location of the next point
		if (CustomPlayerController)
		{
			//Used to decide on which method of teleportation should be used. - We will use the locomotion point as the actual location you teleport to
			CustomPlayerController->SetLocationToMoveAndRotation(LocomotionPoints[CurrentSection]->GetActorLocation(), LocomotionPoints[CurrentSection]->GetActorRotation());
			CustomPlayerController->SetModiferState(ModType, DataInstance->GetLocomotionType());
			CustomPlayerController->MovePlayerViaNavManagerTeleport();
		}

		//Hide the pad that was just shot, is no longer needed
		PointAndTPAreas[CurrentSection]->ShowTeleportPad(false);
	}
	
	//Anything related to teleport will be done here too

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
	}

	//TELEPORT ONLY FOR LOCOMOTION MOVING ON
	//We don't need to change this in the UI for updating as the player has officially teleported and arrived at the point. See "tick" for the same type of logic
	if (DataInstance->GetLocomotionType() == ELocomotionType::POINT_AND_TELEPORT)
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

		//UE_LOG(LogTemp, Error, TEXT("Should send index %d to enemy spawner"), CurrentSection);
		//TODO: Add a delay based on when the player is in the correct position
		EnemySpawner->UpdateSection(CurrentSection);
	}
}

void ANavigationManager::RevealNextLocomotionArrow(int junk) 
{
	//We check + 1 as we have no currently updated the current sub stage so we need to check if there is another point beyond this one

	//DataInstance->SetLocomotionType(ELocomotionType::NODE_BASED);

	if (DataInstance->GetLocomotionType() == ELocomotionType::NODE_BASED) 
	{
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

				//Plays the audio if it's bound
				if (EndOfStageAudioToggle.IsBound())
				{
					EndOfStageAudioToggle.Broadcast(0);
				}
			}
			return;
		}
		//Show the locomotion arrow as we are in node based 
		LocomotionPoints[CurrentSection]->ShowArrow(true);
		TimeManager->PauseOrResumeTimer(true);

		//Send over the data now as we need it before the delegate is called for rotations and locations
		CustomPlayerController->SetLocationToMoveAndRotation(LocomotionPoints[CurrentSection]->GetActorLocation(), LocomotionPoints[CurrentSection]->GetActorRotation());
		//CustomPlayerController->SetModiferState(ModType, DataInstance->GetLocomotionType());

	}
	else if (DataInstance->GetLocomotionType() == ELocomotionType::POINT_AND_TELEPORT)
	{
		//TODO: DO THE SAME AS ABOVE BUT ABOUT THE TELEPORT PLATFORMS
		if (PointAndTPAreas.Num() <= 0 || CurrentSection >= PointAndTPAreas.Num())
		{
			//We have ran out of points, TODO: Launch a delegate to show the end of game data
			UE_LOG(LogTemp, Error, TEXT("End of PontAndTP Areas on %s"), *this->GetName());

			//Lets get the time after pausing at the end of an area/game
			TimeManager->PauseOrResumeTimer(true);

			//Let's allow it to collect all data for the display
			if (AllowDataCollectionFromClassesDelegate.IsBound())
			{
				AllowDataCollectionFromClassesDelegate.Broadcast(0);

				//Plays the audio if it's bound
				if (EndOfStageAudioToggle.IsBound())
				{
					EndOfStageAudioToggle.Broadcast(0);
				}
			}
			return;
		}
		//Show the teleport pad as we are in point and teleport
		PointAndTPAreas[CurrentSection]->ShowTeleportPad(true);

		//Only show the annoation arrow if required
		if (ModType == EModifierTypes::ANNOTATED_ROTATION) 
		{
			PointAndTPAreas[CurrentSection]->ShowTeleportPadAnnotation(true);
		}
		else 
		{
			PointAndTPAreas[CurrentSection]->ShowTeleportPadAnnotation(false);
		}

		TimeManager->PauseOrResumeTimer(true);

		//Allows the point and teleportation to be used when all enemies are dead
		CustomPlayerController->AllowTeleportationAfterAllEnemiesAreDead();

		//Send over the data now as we need it before the delegate is called for rotations and locations
		CustomPlayerController->SetLocationToMoveAndRotation(LocomotionPoints[CurrentSection]->GetActorLocation(), LocomotionPoints[CurrentSection]->GetActorRotation());
		CustomPlayerController->SetModiferState(ModType, DataInstance->GetLocomotionType());
	}

	//Passes the location 
	CustomPlayerController->PassNextSectionLocationToPlayer(LocomotionPoints[CurrentSection]->GetActorLocation());

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

