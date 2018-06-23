// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"

void AMainPlayerController::MovePlayerViaNavManagerTeleport()
{
	//For now lets just teleport to test the logic

	//FRotator PawnsRot = this->GetPawn()->GetActorRotation();

	this->GetPawn()->SetActorLocation(FVector(LocomotionToMoveTowards.X, LocomotionToMoveTowards.Y, 0.0f));
	//this->GetPawn()->SetActorRelativeRotation(FRotator(PawnsRot.Pitch, RotationToFace.Yaw, PawnsRot.Roll));
	
}

void AMainPlayerController::SetLocationToMoveAndRotation(FVector LocationToMoveTo, FRotator RotationToFace) 
{
	LocomotionToMoveTowards = LocationToMoveTo;
	RotationToFaceTowards = RotationToFace;
}

void AMainPlayerController::SetModiferState(EModifierTypes ModiferType, ELocomotionType LocomotionType) 
{
	//TODO: Currently no modifiers being applied
	if (LocomotionType == ELocomotionType::POINT_AND_TELEPORT) 
	{
		//Just setting it to false incase it's a different type
		//bFadeModifierEnabled = false;

		//Is now being done in the VR pawn via the blueprint for now
		//this->MovePlayerViaNavManagerTeleport();
	}
	else if (LocomotionType == ELocomotionType::NODE_BASED)
	{
		this->MovePlayerViaNavManagerNodeBased();
	}
}

void AMainPlayerController::SetIfAllowedToTeleportViaArcForDuration(bool bCanTeleportViaArc) 
{
	bAllowToTeleportThisGame = bCanTeleportViaArc;
}

void AMainPlayerController::AllowTeleportationAfterAllEnemiesAreDead() 
{
	bCanTeleportAfterAllEnemiesDead = true;

	
}

void AMainPlayerController::UpdateSectionViaNavManager() 
{
	//Moves the actor
	//UE_LOG(LogTemp, Error, TEXT("Loco Set on finalised %s"), *LocomotionToMoveTowards.ToString());
	//this->GetPawn()->SetActorLocation(FVector(LocomotionToMoveTowards.X, LocomotionToMoveTowards.Y, 0.0f));

	//Stop the arc teleporter to be used
	bCanTeleportAfterAllEnemiesDead = false;

	if (TeleportLocomodeDataBaseUpdate.IsBound()) 
	{
		TeleportLocomodeDataBaseUpdate.Broadcast();
	}
}




