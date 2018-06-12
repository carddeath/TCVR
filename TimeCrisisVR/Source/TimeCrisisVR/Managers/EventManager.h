// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShowReload, bool, ShouldShowReload);

UCLASS()
class TIMECRISISVR_API AEventManager : public AActor
{
	GENERATED_BODY()

	//Variables
public:

	//Used by the navigation manager to decide if to require the keypad or not
	bool bDoorBeganToClose = false;

	//Used to enable door code check for a limited amount of time
	bool bCheckForCorrectCode = true;

	FShowReload ReloadDisplayDelegate;

protected:

	UPROPERTY(EditInstanceOnly, Category = "Interactable Props")
	class ACraneHook* CraneHookStage1Area1 = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Interactable Props")
	class ASubmarine* SubmarineStage1Area1 = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Interactable Props")
		class AHangerDoors* HangerDoorsStage1Area1 = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Interactable Props")
		class AKeypad* KeyPad = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Timings")
		float TimeBeforeDoorCloses = 10.0f;

private:

	FTimerHandle DelayOnCraneMovementHandle;
	FTimerHandle DelayOnDoorClosing;

	//The manager of navigation for communications
	class ANavigationManager* NavManager;

	class AAnnouncer* Announcer = nullptr;

	class ATutorialManager* TutorialManager = nullptr;

	class AEnemySpawner* EnemySpawner = nullptr;

	//Methods
	
public:	
	AEventManager();
	virtual void Tick(float DeltaTime) override;

	void BlowUpSubmarineSection1();

	void StartTimerOnHangerDoorToClose();

	void StopTimerOnHangerDoor();

	UFUNCTION()
	void ShowEndOfGameUIViaUIManager(int junk);

	void AnnouncerCallThroughAction();

	void AnnouncerReloadCallThroughAndUIBroadcast();

	//Tutorial Functions
	void LoadMainLevelFromTutorial();

protected:
	virtual void BeginPlay() override;

private:	

	//Animates the crane at the start of the game after a delay
	void MoveCraneStage1Area1Section1();
	
	void CloseDoorAfterTimerStage1Area1Section3(bool bShouldClose);

	void PlayDoorCodeAudio();
};
