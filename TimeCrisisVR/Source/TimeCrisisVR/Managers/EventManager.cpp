// Fill out your copyright notice in the Description page of Project Settings.

#include "EventManager.h"
#include "EngineUtils.h"
#include "Public/TimerManager.h"

#include "Managers/NavigationManager.h"
#include "Events/CraneHook.h"

// Sets default values
AEventManager::AEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEventManager::BeginPlay()
{
	Super::BeginPlay();

	//Get a reference to the navigation manager
	for (TActorIterator<ANavigationManager> it(GetWorld()); it; ++it) 
	{
		NavManager = *it;

		if (!NavManager) 
		{
			UE_LOG(LogTemp, Error, TEXT("Missing Navigation Manager on %s"), *this->GetName());
		}
	}

	//If we are at the start of the game
	if (NavManager) 
	{
		if (NavManager->GetCurrentGameStage() == 1 && NavManager->GetCurrentGameArea() == 1 && NavManager->GetCurrentGameSection() == 1)
		{
			GetWorldTimerManager().SetTimer(DelayOnCraneMovementHandle, this, &AEventManager::MoveCraneStage1Area1Section1, 3.0f, false);
		}
	}
}

// Called every frame
void AEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEventManager::MoveCraneStage1Area1Section1() 
{
	if (CraneHookStage1Area1) 
	{
		CraneHookStage1Area1->StartCraneAnimationSequence();
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Crane Hook Stage 1 Area 1 on %s"), *this->GetName());
	}
}

