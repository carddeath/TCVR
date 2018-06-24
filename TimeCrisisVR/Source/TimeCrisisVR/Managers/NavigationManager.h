// Fill out your copyright notice in the Description page of Project Settings.

/*
Responsible for exposing the correct locomotion position when the enemy spawner tells this nav manager that all are dead. 
Should also tell the enemy spawner to update it's enemy set based on the, level, act and stage
*/

#pragma once

#include "Data/TutorialToGameSaveInstance.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavigationManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFlashUpAreaAndWait, int32, StageNum);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisplayProceedOnAreaClear, bool, bShouldShowProceed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDisplayWait, bool, bShouldShowWait);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReachedEndOfArea, bool, bShouldShowEndOfAreaWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReachedEndOfAreaDataTransfer, int32, junk);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReactiveGun, int32, junk);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayEndOfStageAudio, int32, junk);

UCLASS()
class TIMECRISISVR_API ANavigationManager : public AActor
{
	GENERATED_BODY()

	//Variables
public:

	//For the start of each stage
	FFlashUpAreaAndWait FlashUpDelegateAreaStart;
	
	//For once an area is clear we should display to the player that they're read to proceed
	FDisplayProceedOnAreaClear ToggleProceedDisplayDelegate;

	//Used to turn off or on the wait ui and say action!
	FDisplayWait ToggleWaitOffDelegate;

	//A delegate that is called when "action delegate" is called as well
	FReactiveGun TurnGunToEnabled;

	//Used for show the end of area widget and turn off the other
	FReachedEndOfArea ShowEndOfAreaWidgetDelegate;

	//Allows the DataTracker to seek out all data required for the end of area stats
	FReachedEndOfAreaDataTransfer AllowDataCollectionFromClassesDelegate;

	//Used by music manager to change audio on game end
	FPlayEndOfStageAudio EndOfStageAudioToggle;

protected:

	//For now this is a target point but it will change in the future
	UPROPERTY(EditInstanceOnly, Category = "Locomotion Points")
	TArray<class ANavigationArrow*> LocomotionPoints;

	//Used for the point and teleport areas instead
	UPROPERTY(EditInstanceOnly, Category = "Locomotion Points")
		TArray<class APlayerConstraintArea*> PointAndTPAreas;

private:

	//Used to allow custom locomotion technique to be loaded when required
	UPROPERTY(EditInstanceOnly, Category = "Locomotion Debug")
		bool bIsInDebug = false;

	class AEnemySpawner* EnemySpawner = nullptr;

	class AEventManager* EventManager = nullptr;

	class ATImeManager* TimeManager = nullptr;

	//The current stage in the game - goes from 1-3
	int32 CurrentStage = 1;

	//The current Act in the game - goes from 1-3
	int32 CurrentArea = 1;

	//The current shooting position of the player. - goes from 0 - max needed
	int32 CurrentSection = 1;

	class AMainPlayerController* CustomPlayerController = nullptr;

	//USed to check in "tick" if the player has reached the next locomotion point. If so then we remove the wait UI and display action
	bool bHasPlayerSetOffToNextPoint = false;

	//The tutorial instance for getting the locomotion type
	UTutorialToGameSaveInstance* DataInstance = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Locomotion Debug")
	EModifierTypes ModType;

	UPROPERTY(EditInstanceOnly, Category = "Locomotion Debug")
	ELocomotionType LocoType;

	float XResult;
	float YResult;

	//used to track when the fade should begin or end on the 
	bool bBeganFadeOnNode = false;
	//Methods
	
public:	
	ANavigationManager();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateCurrentSection();

	int32 GetCurrentGameStage();
	int32 GetCurrentGameArea();
	int32 GetCurrentGameSection();

	void BroadCastStageArea();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Node Fading")
		void FadeOnNodeLocomotion(bool bShouldFadeDark);

private:	

	UFUNCTION()
	void RevealNextLocomotionArrow(int junk);

	void EventChecker();

	void SetLocomotionModifer();


	
	
};
