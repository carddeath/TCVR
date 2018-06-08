// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataStructures/GameData.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTracker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSendDataToUIElement, FGameData, data);

UCLASS()
class TIMECRISISVR_API ADataTracker : public AActor
{
	GENERATED_BODY()

	//Variables
public:

	//Sends the data to the ui when required
	FSendDataToUIElement DataSendingDelegate;

protected:

private:

	class APlayersGun* PlayersGun = nullptr;

	class ANavigationManager* NavManager = nullptr;

	class ATImeManager* TimeManager = nullptr;

	FGameData GameData;

	//Methods
	
public:	

	ADataTracker();
	virtual void Tick(float DeltaTime) override;
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
		void FindAllData(int junk);

private:	


	
};
