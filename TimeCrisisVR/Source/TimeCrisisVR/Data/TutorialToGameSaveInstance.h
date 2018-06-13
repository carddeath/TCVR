// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures/Hand.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TutorialToGameSaveInstance.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UTutorialToGameSaveInstance : public UGameInstance
{
	GENERATED_BODY()

	//Variables
public:

protected:

private:

	EHand PlayerHandPreference;

	//Methods
public:

	virtual void Init();

	EHand GetPreferedHand();

	UFUNCTION()
		void SetHandTypeViaDelegation(EHand HandType);

protected:

private:
	
	
	
};
