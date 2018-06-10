// Fill out your copyright notice in the Description page of Project Settings.

#include "EventManager.h"
#include "EngineUtils.h"
#include "Public/TimerManager.h"

#include "Managers/NavigationManager.h"
#include "Events/CraneHook.h"
#include "Events/Submarine.h"
#include "Player/VRPawn.h"
#include "Events/HangerDoors.h"
#include "Interactables/Keypad.h"
#include "Gameplay/Announcer.h"
#include "Kismet/GameplayStatics.h"

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

	for (TActorIterator<AAnnouncer> it(GetWorld()); it; ++it)
	{
		Announcer = *it;

		if (!Announcer)
		{
			UE_LOG(LogTemp, Error, TEXT("Missing Announcer on %s"), *this->GetName());
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

	//THIS IS DIRTY WAY OF DOING IT. THINK OF SOMETHING BETTER. We will open the door if they entered the correct code
	if (NavManager) 
	{
		if (NavManager->GetCurrentGameStage() == 1 && NavManager->GetCurrentGameArea() == 1 && NavManager->GetCurrentGameSection() == 4 && bCheckForCorrectCode)
		{

			if (KeyPad->GetCurrentIndexOfEnteredNum() >= 4 && KeyPad->CheckIfCodeIsCorrect())
			{
				bCheckForCorrectCode = false;
				StopTimerOnHangerDoor();
			}
		}
	}

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

void AEventManager::BlowUpSubmarineSection1() 
{
	if (SubmarineStage1Area1) 
	{
		SubmarineStage1Area1->StartSubmarineSinkSequence();
		Cast<AVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn())->TurnAlarmOnOrOff(true);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Missing Submarine Stage 1 Area 1 on %s"), *this->GetName());
	}
}

void AEventManager::StartTimerOnHangerDoorToClose() 
{
	FTimerDelegate DoorCloseTimerDele = FTimerDelegate::CreateUObject(this, &AEventManager::CloseDoorAfterTimerStage1Area1Section3, true);
	GetWorldTimerManager().SetTimer(DelayOnDoorClosing, DoorCloseTimerDele, TimeBeforeDoorCloses, false);
}

void AEventManager::StopTimerOnHangerDoor() 
{
	if (NavManager->GetCurrentGameStage() == 1 && NavManager->GetCurrentGameArea() == 1 && NavManager->GetCurrentGameSection() == 4) 
	{
		GetWorldTimerManager().ClearTimer(DelayOnDoorClosing);
		CloseDoorAfterTimerStage1Area1Section3(false);
		Cast<AVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn())->TurnAlarmOnOrOff(false);

		//Disable the keys of the pad and star the display, we don't need to interact with it
		KeyPad->StarOutAndDisableButton();
	}
}

void AEventManager::CloseDoorAfterTimerStage1Area1Section3(bool bShouldClose) 
{
	if (HangerDoorsStage1Area1) 
	{
		bDoorBeganToClose = true;
		HangerDoorsStage1Area1->ShouldCloseDoors(bShouldClose);

		//Only do this if the door should close
		if (bShouldClose) 
		{
			//Make the keypad play the audio cue every 8 seconds
			FTimerHandle DoorCodeHandle;
			GetWorldTimerManager().SetTimer(DoorCodeHandle, KeyPad, &AKeypad::PlayDoorAudioCueCPP, 1.0f);
		}
	}
}

void AEventManager::ShowEndOfGameUIViaUIManager(int junk) 
{
	UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
}

void AEventManager::AnnouncerCallThroughAction() 
{
	Announcer->PlayAction();
}

void AEventManager::AnnouncerReloadCallThroughAndUIBroadcast() 
{
	Announcer->PlayReload();

	//We want to show the reload text
	if (ReloadDisplayDelegate.IsBound()) 
	{
		ReloadDisplayDelegate.Broadcast(true);
	}
}

//Tutorial Logic
void AEventManager::LoadMainLevelFromTutorial() 
{
	FName LevelToLoadName = "Main_Scene";
	UGameplayStatics::OpenLevel(this, LevelToLoadName, false);
}

