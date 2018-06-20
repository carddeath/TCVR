// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialToGameSaveInstance.h"

void UTutorialToGameSaveInstance::Init() 
{
	//The hand type hasn't been assigned most likely. Default it to Right as most people are right handed
	if (PlayerHandPreference != EHand::LEFT || PlayerHandPreference != EHand::RIGHT) 
	{
		PlayerHandPreference = EHand::RIGHT;
	}
}

//Setters

void UTutorialToGameSaveInstance::SetHandTypeViaDelegation(EHand HandType) 
{
	PlayerHandPreference = HandType;
}

void UTutorialToGameSaveInstance::SetLocomotionType(ELocomotionType LocoType) 
{
	CurrentLocomotionType = LocoType;
}

void UTutorialToGameSaveInstance::SetCurrentTrial(int32 CurTrial) 
{
	CurrentTrail = CurTrial;
}

//Getters

EHand UTutorialToGameSaveInstance::GetPreferedHand() 
{
	return PlayerHandPreference;
}

ELocomotionType UTutorialToGameSaveInstance::GetLocomotionType() 
{
	return CurrentLocomotionType;
}

int32 UTutorialToGameSaveInstance::GetCurrentTrail() 
{
	return CurrentTrail;
}
