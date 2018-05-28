// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	//Variables
public:

protected:

private:

	//Methods
public:

	//For Spawn, Run and Shoot soldiers
	UFUNCTION(BlueprintImplementableEvent, Category = "Navigation")
		void MoveToPosition(class ATargetPoint* GoToPoint);

	//For spawn, run, shoot and escape soldiers
	UFUNCTION(BlueprintImplementableEvent, Category = "Navigation")
		void SpawnMoveShootEscape(ATargetPoint* GoToPoint ,ATargetPoint* EscapePoint);

	//FOr Spawn, pop up from crouch, shoot, advance and shoot soldiers
	UFUNCTION(BlueprintImplementableEvent, Category = "Navigation")
		void SpawnPopShootAdvanceShoot(ATargetPoint* AdvancePoint);

	//For enemies that pop up and just shoot at the player
	UFUNCTION(BlueprintImplementableEvent, Category = "Navigation")
		void SpawnPopShoot();

protected:


private:
	
	
	
};
