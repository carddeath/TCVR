// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "CraneHook.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API ACraneHook : public ASkeletalMeshActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

private:
	
	//Methods
public:
	//Simply sets the bool to true in the animation instance, called from the event manager
	void StartCraneAnimationSequence();

protected:

private:
	
	
};
