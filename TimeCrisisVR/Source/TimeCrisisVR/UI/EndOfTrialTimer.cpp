// Fill out your copyright notice in the Description page of Project Settings.

#include "EndOfTrialTimer.h"
#include "Kismet/GameplayStatics.h"
#include "Data/TutorialToGameSaveInstance.h"
#include "Engine.h"
#include "TimeCrisisVRGameModeBase.h"
#include <iostream>
#include <fstream>


void UEndOfTrialTimer::SetCountTimerDown(float TotalTimeTaken) 
{
	bCountTimerDown = true;
	TotalTimeTakenInStage = TotalTimeTaken;
}

void UEndOfTrialTimer::GenerateNewLocomotionModifier() 
{

	DataInstance = Cast<UTutorialToGameSaveInstance>(GetWorld()->GetGameInstance());

	if (!DataInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Data Instance on %s"), *this->GetName());
		return;
	}

	//Gets the current trail number to pick which trial to experiment with
	int32 CurrentIndex = DataInstance->GetCurrentTrail();

	FileName.Empty();
	PickLocomotionTypeToWrite();

	//Setting and saving the new modifier for the next trial
	if (CurrentIndex == 3) 
	{
		//REset the datainstance here
		DataInstance->SetCurrentTrial(0);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("DataInstance Trail is now: %d"), DataInstance->GetCurrentTrail()));
		//TODO: QUIT OR GO BACK TO THE MAIN MENU where you select the mode
		
		UE_LOG(LogTemp, Error, TEXT("QUIT THE GAME"));

		Cast<ATimeCrisisVRGameModeBase>(GetWorld()->GetAuthGameMode())->LoadLocomotionMainLevel();
	}
	else 
	{
		Cast<ATimeCrisisVRGameModeBase>(GetWorld()->GetAuthGameMode())->LoadNewLevel();
	}

	//Increment the current index and we write it if we are not over 3
	CurrentIndex++;

	DataInstance->SetCurrentTrial(CurrentIndex);

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("DataInstance Trail is now: %d"), DataInstance->GetCurrentTrail()));
	//TODO: Write all the times and the name of the technique to a file 

	//TODO: DO a comparisson here of the number, if the trial number is 4. Do not load the level. Quit the game
}

void UEndOfTrialTimer::PickLocomotionTypeToWrite() 
{
	if (DataInstance->GetLocomotionType() == ELocomotionType::POINT_AND_TELEPORT) 
	{
		FileName.Append("PAT_");
	}
	else if (DataInstance->GetLocomotionType() == ELocomotionType::NODE_BASED)
	{
		FileName.Append("NB_");
	}

	PickModiferTypeToWrite();
}

void UEndOfTrialTimer::PickModiferTypeToWrite() 
{
	FString TimerStringWithNewLine;
	//FString SaveDirectory = FString("F:/GSD2017Work/Masters/Trimester 3/Final Project/DATA");
	FString SaveDirectory = FPaths::GameDir();
	SaveDirectory += "DATA";
	FString FinalisedFilePath;
	UE_LOG(LogTemp, Warning, TEXT("Total time taken is %f"), TotalTimeTakenInStage);

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	//Used for writing to file
	switch (DataInstance->GetCurrentTrail())
	{
		//None
	case 0:
		UE_LOG(LogTemp, Error, TEXT("No modifer applied"));
		FileName.Append("no_modifiers.txt");
		break;
		//Fade
	case 1:
		UE_LOG(LogTemp, Error, TEXT("Fade applied"));
		FileName.Append("fade_applied.txt");
		break;
		//Forced rotation
	case 2:
		UE_LOG(LogTemp, Error, TEXT("Forced Rotation applied"));
		FileName.Append("forced_rotation.txt");
		break;
		//Annotated rotation
	case 3:
		UE_LOG(LogTemp, Error, TEXT("Annotated Rotation applied"));
		FileName.Append("annotated_rotation.txt");
		break;
	default:
		//We return because we don't want to write the file
		return;
	}

	//If it was 0 - 3 we will write the file, otherwise we return
	//If we don't have the directory
	if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
	{
		FinalisedFilePath = SaveDirectory + "/" + FileName;
		TimerStringWithNewLine = FString::SanitizeFloat(TotalTimeTakenInStage);
		TimerStringWithNewLine += LINE_TERMINATOR;
		FFileHelper::SaveStringToFile(TimerStringWithNewLine, *FinalisedFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}
}
