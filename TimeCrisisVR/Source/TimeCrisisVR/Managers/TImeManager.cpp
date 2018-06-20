// Fill out your copyright notice in the Description page of Project Settings.

#include "TImeManager.h"
#include "Player/VRPawn.h"
#include "UI/Timer.h"
#include "Managers/EventManager.h"


// Sets default values
ATImeManager::ATImeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATImeManager::BeginPlay()
{
	Super::BeginPlay();

	UUserWidget* Temp = Cast<AVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetWatchClass();
	TimerWatchWidget = Cast<UTimer>(Temp);

	if (TimerWatchWidget && EventManager) 
	{
		TimerWatchWidget->EndOfGameTriggerDelegate.AddDynamic(EventManager, &AEventManager::ShowEndOfGameUIViaUIManager);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Missing time watch widget or event manager on %s"), *this->GetName());
	}

	TotalTimeTakenInTheGame = 0.0f;
	
}

// Called every frame
void ATImeManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATImeManager::AddTimeToTimersForSectionBasedOnArea(int32 StageNumber, int32 Area) 
{
	//Different stages and areas have different times to add
	if (StageNumber == 1) 
	{
		switch (Area) 
		{
		case 1:
			TimerWatchWidget->AddTimeToTotal(20.0f);
			break;
		case 2:
			TimerWatchWidget->AddTimeToTotal(10.0f);
			break;
		case 3:
			TimerWatchWidget->AddTimeToTotal(20.0f);
			break;
		}
	}
}

void ATImeManager::PauseOrResumeTimer(bool bShouldPause)
{
	TimerWatchWidget->TogglePauseOnTimer(bShouldPause);

	//Update the total time taken
	if (bShouldPause) 
	{
		TotalTimeTakenInTheGame = TimerWatchWidget->GetTotalTimeTaken();
	}
}

float ATImeManager::GetTotalTimeAtAreaEnd() 
{
	//Get the total time taken, reset the counter and return the actual value
	float HolderOfTime = TotalTimeTakenInTheGame;
	return HolderOfTime;
}

