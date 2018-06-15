// Fill out your copyright notice in the Description page of Project Settings.

#include "EndOfTrialTimer.h"
#include "Kismet/GameplayStatics.h"


void UEndOfTrialTimer::SetCountTimerDown() 
{
	bCountTimerDown = true;
}

void UEndOfTrialTimer::GenerateNewLocomotionModifier() 
{
	//TODO: Write the new state, that should be loaded in the navigation manager and write the current trial number, if we go to 4 then we should display a different message in this widget to take the headset off
	//TODO: Write all the times and the name of the technique to a file 

	//TODO: DO a comparisson here of the number, if the trial number is 4. Do not load the level. Quit the game
	LoadLevelAgain();
}

void UEndOfTrialTimer::LoadLevelAgain() 
{
	//UGameplayStatics::Level
	//FName LevelToLoadName = "Main_Scene";
	//UGameplayStatics::OpenLevel(this, LevelToLoadName, false);
}
