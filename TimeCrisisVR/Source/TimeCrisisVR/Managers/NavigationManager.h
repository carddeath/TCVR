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
	UPROPERTY(EditInstanceOnly, Category = "Locomotion Points")
	TArray<class ANavigationArrow*> LocomotionPoints;

private:

	class AEnemySpawner* EnemySpawner = nullptr;

	class AEventManager* EventManager = nullptr;

	//The current stage in the game - goes from 1-3
	int32 CurrentStage = 1;

	//The current Act in the game - goes from 1-3
	int32 CurrentArea = 1;

	//The current shooting position of the player. - goes from 0 - max needed
	int32 CurrentSection = 1;

	class AMainPlayerController* CustomPlayerController = nullptr;

	//Methods
	
public:	
	ANavigationManager();
	virtual void Tick(float DeltaTime) override;

	void UpdateCurrentSection(bool bTeleportPlayer);

	int32 GetCurrentGameStage();
	int32 GetCurrentGameArea();
	int32 GetCurrentGameSection();

protected:
	virtual void BeginPlay() override;

private:	

	UFUNCTION()
	void RevealNextLocomotionArrow(int junk);

	void EventChecker();


	
	
};
