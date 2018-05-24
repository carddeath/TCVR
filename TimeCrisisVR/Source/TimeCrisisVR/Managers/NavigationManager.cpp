// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigationManager.h"

#include "Managers/EnemySpawner.h"
#include "Engine.h"
#include "Player/MainPlayerController.h"
#include "Gameplay/NavigationArrow.h"

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

	EnemySpawner->RevealNextPointToNavMang.AddDynamic(this, &ANavigationManager::RevealNextLocomotionArrow);

	//Save a reference to the player controller
	CustomPlayerController = Cast<AMainPlayerController>(GetWorld()->GetFirstPlayerController());

	//Disable all arrows
	for (auto& Arrow : LocomotionPoints) 
	{
		Arrow->ShowArrow(false);
	}
	
}

// Called every frame
void ANavigationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANavigationManager::UpdateCurrentSubStage(bool bTeleportPlayer) 
{
	//Increment the current sub stage
	CurrentSubStage++;

	if (LocomotionPoints.Num() <= 0 || CurrentSubStage >= LocomotionPoints.Num())
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
			CustomPlayerController->MovePlayerViaNavManagerNodeBased(LocomotionPoints[CurrentSubStage]->GetActorLocation(), LocomotionPoints[CurrentSubStage]->GetActorRotation());
		}
		else 
		{
			CustomPlayerController->MovePlayerViaNavManagerTeleport(LocomotionPoints[CurrentSubStage]->GetActorLocation(), LocomotionPoints[CurrentSubStage]->GetActorRotation());
		}
	}

	//Hide the arrow that was just shot, is no longer needed
	LocomotionPoints[CurrentSubStage]->ShowArrow(false);

	//TODO: Add a delay based on when the player is in the correct position
	EnemySpawner->UpdateSubStage(CurrentSubStage);
}

void ANavigationManager::RevealNextLocomotionArrow(int junk) 
{
	//We check + 1 as we have no currently updated the current sub stage so we need to check if there is another point beyond this one
	if (LocomotionPoints.Num() <= 0 || CurrentSubStage + 1 >= LocomotionPoints.Num()) 
	{
		UE_LOG(LogTemp, Error, TEXT("End of locomotion points on %s"), *this->GetName());
		return;
	}
		LocomotionPoints[CurrentSubStage + 1]->ShowArrow(true);
}

