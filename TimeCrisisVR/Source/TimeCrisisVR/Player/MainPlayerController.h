// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/TutorialToGameSaveInstance.h"

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

		//Variables
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Values")
	EModifierTypes ModiferType;

	UPROPERTY(BlueprintReadOnly, Category = "Values")
		FVector LocomotionToMoveTowards;

	UPROPERTY(BlueprintReadOnly, Category = "Values")
		FRotator RotationToFaceTowards;
		//Methods
public:

	void SetModiferState(EModifierTypes ModiferType, ELocomotionType LocomotionType);

	void SetLocationToMoveAndRotation(FVector LocationToMoveTo, FRotator RotationToFace);

	UFUNCTION(BlueprintImplementableEvent, Category = "Locomotion")
		void MovePlayerViaNavManagerNodeBased();

		void MovePlayerViaNavManagerTeleport();

protected:

	
};
