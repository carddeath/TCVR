// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"

void AMainPlayerController::MovePlayerViaNavManagerTeleport()
{
	//For now lets just teleport to test the logic

	FRotator PawnsRot = this->GetPawn()->GetActorRotation();

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
	if (LocomotionType == ELocomotionType::POINT_AND_TELEPORT) 
	{
		this->MovePlayerViaNavManagerTeleport();
	}
	else if (LocomotionType == ELocomotionType::NODE_BASED)
	{
		this->MovePlayerViaNavManagerNodeBased();
	}
}




