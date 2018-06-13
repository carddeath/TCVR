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

void UTutorialToGameSaveInstance::SetHandTypeViaDelegation(EHand HandType) 
{
	PlayerHandPreference = HandType;
}

EHand UTutorialToGameSaveInstance::GetPreferedHand() 
{
	return PlayerHandPreference;
}
