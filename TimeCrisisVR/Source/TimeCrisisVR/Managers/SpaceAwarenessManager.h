// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceAwarenessManager.generated.h"

UCLASS()
class TIMECRISISVR_API ASpaceAwarenessManager : public AActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

private:

	//Methods
	
public:	
	ASpaceAwarenessManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	

};
