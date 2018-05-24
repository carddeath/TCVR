// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "NavigationArrow.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API ANavigationArrow : public AStaticMeshActor
{
	GENERATED_BODY()

	//Variables
public:



protected:

private:

	//If the object is visible
	bool bIsVisible = false;

	//Methods

public:

	void ShowArrow(bool bShouldShow);

protected:

private:
	
	
	
	
};
