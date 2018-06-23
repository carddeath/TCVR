// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures/Hand.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TutorialToGameSaveInstance.generated.h"

UENUM(Blueprintable)
enum class EModifierTypes : uint8 
{
	NONE,
	FADE,
	FORCED_ROTATION,
	ANNOTATED_ROTATION
};

UENUM(Blueprintable)
enum class ELocomotionType : uint8
{
	POINT_AND_TELEPORT,
	NODE_BASED
};

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


	UPROPERTY(BlueprintReadWrite, Category = "Tutorial state")
		bool bWasInitialTutorialDone = false;


	UPROPERTY(BlueprintReadOnly, Category = "Mods")
		int32 CurrentTrail;

		ELocomotionType CurrentLocomotionType;

private:

	EHand PlayerHandPreference;



	//Methods
public:

	virtual void Init();

	EHand GetPreferedHand();

	ELocomotionType GetLocomotionType();

	int32 GetCurrentTrail();

	UFUNCTION()
		void SetHandTypeViaDelegation(EHand HandType);


	UFUNCTION(BlueprintCallable, Category = "Experiment")
		void SetLocomotionType(ELocomotionType LocoType);

		UFUNCTION(BlueprintCallable, Category = "Experiment")
		void SetCurrentTrial(int32 CurTrial);

protected:

private:
	
	
	
};
