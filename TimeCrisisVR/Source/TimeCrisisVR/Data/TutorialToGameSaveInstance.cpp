// Fill out your copyright notice in the Description page of Project Settings.

#include "TutorialToGameSaveInstance.h"

void UTutorialToGameSaveInstance::SetHandTypeViaDelegation(EHand HandType) 
{
	PlayerHandPreference = HandType;
}

EHand UTutorialToGameSaveInstance::GetPreferedHand() 
{
	return PlayerHandPreference;
}
