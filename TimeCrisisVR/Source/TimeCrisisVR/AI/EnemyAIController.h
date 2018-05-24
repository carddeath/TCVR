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

	UFUNCTION(BlueprintImplementableEvent, Category = "Navigation")
		void MoveToPosition(class ATargetPoint* GoToPoint);

protected:

private:
	
	
	
};
