// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine.h"

void AMainPlayerController::MovePlayerViaNavManagerTeleport(FVector LocationToMove, FRotator RotationToFace)
{
	//For now lets just teleport to test the logic

	FRotator PawnsRot = this->GetPawn()->GetActorRotation();

	this->GetPawn()->SetActorLocation(FVector(LocationToMove.X, LocationToMove.Y, 0.0f));
	this->GetPawn()->SetActorRelativeRotation(FRotator(PawnsRot.Pitch, RotationToFace.Yaw, PawnsRot.Roll));
	
}




