// Fill out your copyright notice in the Description page of Project Settings.

/*
Responsible for exposing the correct locomotion position when the enemy spawner tells this nav manager that all are dead. 
Should also tell the enemy spawner to update it's enemy set based on the, level, act and stage
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationManager.generated.h"



UCLASS()
class TIMECRISISVR_API ANavigationManager : public AActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

	//For now this is a target point but it will change in the future
	UPROPERTY(EditAnywhere, Category = "Locomotion Points")
	TArray<class ANavigationArrow*> LocomotionPoints;

private:

	class AEnemySpawner* EnemySpawner = nullptr;

	//The current level in the game
	int32 CurrentLevel = 0;

	//The current Act in the game
	int32 CurrentAct = 1;

	//The current stage of the level
	int32 CurrentStage = 0;

	//The current shooting position of the player
	int32 CurrentSubStage = 0;

	class AMainPlayerController* CustomPlayerController = nullptr;

	//Methods
	
public:	
	ANavigationManager();
	virtual void Tick(float DeltaTime) override;

	void UpdateCurrentSubStage(bool bTeleportPlayer);

protected:
	virtual void BeginPlay() override;

private:	

	UFUNCTION()
	void RevealNextLocomotionArrow(int junk);


	
	
};
