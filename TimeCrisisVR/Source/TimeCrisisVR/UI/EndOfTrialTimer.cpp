// Fill out your copyright notice in the Description page of Project Settings.

#include "EndOfTrialTimer.h"
#include "Kismet/GameplayStatics.h"
#include "Data/TutorialToGameSaveInstance.h"
#include "Engine.h"
#include "TimeCrisisVRGameModeBase.h"

void UEndOfTrialTimer::SetCountTimerDown() 
{
	bCountTimerDown = true;
}

void UEndOfTrialTimer::GenerateNewLocomotionModifier() 
{

	UTutorialToGameSaveInstance* DataInstance = Cast<UTutorialToGameSaveInstance>(GetWorld()->GetGameInstance());

	if (!DataInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Data Instance on %s"), *this->GetName());
		return;
	}

	//Gets the current trail number to pick which trial to experiment with
	int32 CurrentIndex = DataInstance->GetCurrentTrail();

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

	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("DataInstance Trail is now: %d"), DataInstance->GetCurrentTrail()));
	//TODO: Write all the times and the name of the technique to a file 

	//TODO: DO a comparisson here of the number, if the trial number is 4. Do not load the level. Quit the game
}
