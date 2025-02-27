// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/TutorialToGameSaveInstance.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateViaTeleportLocoMode);

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

		//Variables
public:

	FUpdateViaTeleportLocoMode TeleportLocomodeDataBaseUpdate;

	UPROPERTY(BlueprintReadWrite, Category = "State")
		bool bCanFireTeleportArc = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsTutorial = false;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Values")
	EModifierTypes ModiferType;

	UPROPERTY(BlueprintReadOnly, Category = "Values")
		FVector LocomotionToMoveTowards;

	UPROPERTY(BlueprintReadOnly, Category = "Values")
		FRotator RotationToFaceTowards;

	//Set from the Navigation manager 
	UPROPERTY(BlueprintReadOnly, Category = "Values")
		bool bAllowToTeleportThisGame = false;

	UPROPERTY(BlueprintReadWrite, Category = "Values")
		bool bCanTeleportAfterAllEnemiesDead = false;

	UPROPERTY(BlueprintReadWrite, Category = "Values")
		bool bFadeModifierEnabled = false;


		//Methods
public:

	void SetModiferState(EModifierTypes ModiferType, ELocomotionType LocomotionType);

	void SetLocationToMoveAndRotation(FVector LocationToMoveTo, FRotator RotationToFace);

	void SetIfAllowedToTeleportViaArcForDuration(bool bCanTeleportViaArc);
	
	UFUNCTION()
	void AllowTeleportationAfterAllEnemiesAreDead();

	UFUNCTION(BlueprintImplementableEvent, Category = "Locomotion")
		void MovePlayerViaNavManagerNodeBased();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Locomotion")
		void DrawTeleportArc();

		void MovePlayerViaNavManagerTeleport();

		UFUNCTION(BlueprintCallable, Category = "TeleportLocomotion")
			void UpdateSectionViaNavManager();

		UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Locomotion")
			void RotatePlayerAfterTeleport();

		//Passes the nav location the player
			void PassNextSectionLocationToPlayer(FVector Location);

			//Instruction passed through to the player
			void DisableNavGuidanceArrowStatus();

			void SetValidTutorialStateForLocomotion();

protected:

	
};
