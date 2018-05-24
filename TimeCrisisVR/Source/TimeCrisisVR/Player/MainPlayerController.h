// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

		//Methods
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Locomotion")
		void MovePlayerViaNavManagerNodeBased(FVector LocationToMoveTo, FRotator RotationToFace);

		void MovePlayerViaNavManagerTeleport(FVector LocationToMove, FRotator RotationToFace);
	
	
	
	
};
