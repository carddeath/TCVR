// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "ArmyJeep.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API AArmyJeep : public ASkeletalMeshActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UChildActorComponent* ExplodingBoxChild = nullptr;

private:

	//Methods

public:
	AArmyJeep();

	void StartExplodedAnimation();

protected:

private:

	
	
	
	
};
