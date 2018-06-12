// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

UCLASS()
class TIMECRISISVR_API ATutorialManager : public AActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UWidgetComponent* TutorialWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	TArray<class UTexture2D*> TutorialImages;

	//The hand selection boxes will be hidden on launch and shown when required
	UPROPERTY(EditInstanceOnly, Category = "Objects")
		TArray<class AHandSelectionBox*> HandSelectionBoxes;

	//Should be a target object for step 4
	UPROPERTY(EditDefaultsOnly, Category = "Tutorial Step 4")
		TSubclassOf<class ATutorialShootingTarget> TargetTemplate = nullptr;

	UPROPERTY(EditInstanceOnly, Category = "Objects")
		class ANavigationArrow* EndLevelArrow = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		class USoundBase* PhaseMoveSFX = nullptr;
	
private:
	class AVRPawn* PlayerCharacter = nullptr;

	class APlayersGun* PlayersGun = nullptr;

	class AEnemySpawner* EnemySpawner = nullptr;

	//The current step of the tutorial
	int32 TutorialStepCounter = 0;

	TArray<FString> TutorialMessages;

	ATutorialShootingTarget* CurrentTarget = nullptr;

	class UTutorialToGameSaveInstance* DataInstance = nullptr;


	//Methods
public:	
	ATutorialManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	
	void GenerateTutorialMessageScreens();

	void AssignDelegates();

	void HideAllObjects();

	//Proceeds the tutorial step
	UFUNCTION()
	void ProceedTutorialStep(int junk);

	UFUNCTION()
		void GetHandSelectionActor(AActor* BoxToHighlight, bool bStartAnim);
};
