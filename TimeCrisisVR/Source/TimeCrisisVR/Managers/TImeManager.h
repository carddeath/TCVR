// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TImeManager.generated.h"

UCLASS()
class TIMECRISISVR_API ATImeManager : public AActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

	UPROPERTY(EditInstanceOnly, Category = "Managers")
		class AEventManager* EventManager = nullptr;

private:

	class UTimer* TimerWatchWidget = nullptr;

	float TotalTimeTakenInTheGame = 0.0f;

	//Methods
public:	
	ATImeManager();
	virtual void Tick(float DeltaTime) override;

	//Called on the navigation manager
	void AddTimeToTimersForSectionBasedOnArea(int32 StageNumber, int32 Area);

	void PauseOrResumeTimer(bool bShouldPause);

	float GetTotalTimeAtAreaEnd();

protected:
	virtual void BeginPlay() override;

private:	


	
};
