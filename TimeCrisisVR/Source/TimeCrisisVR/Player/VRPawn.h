// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGunWasCreated, class APlayersGun*, PGun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTutorialProceed, int32, junk);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBeginBoxStare, AActor*, SelectedBox, bool, bStartAnimation);

UENUM()
enum class EHand : uint8 
{
	LEFT,
	RIGHT,
	NONE
};

UCLASS()
class TIMECRISISVR_API AVRPawn : public APawn
{
	GENERATED_BODY()

	//Variables

public:

	//Camera and it's root. Only ever move the root. Should be parented together
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
		class USceneComponent* VRCameraRoot = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VR")
		class UCameraComponent* VRCamera = nullptr;

	//Used so the data manager can get the gun
	FGunWasCreated GunWasCreatedDelegate;


	//TUTORIAL LOGIC VARIABLES
	//Tutorial delegate to broadcast when the button is pressed, if it's bound
	FTutorialProceed TutorialProceedDelegate;

	//Both being used to select which hand should be used for the gun
	FBeginBoxStare BeginBoxStareAnimation;

	bool bIsTutorial = false;
	bool bTutorialIsSearchingForHands = false;
	bool bTutoialStartedAnimHandBox = false;

//The actor that was hovered over last
	AActor* TutorialHoveredHandChoiceBox = nullptr;

	//TODO: Assign this and pass it to the function that creates a gun and send the component and actor we want to assign the gun to so we only need one function.
	EHand TutorialChosenHand;

	//Step 3 of the tutorial
	bool bTutorialDidSwapHands = false;

protected:


	//Motion Controllers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR")
	class UMotionControllerComponent* MCLeft = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "VR")
	UMotionControllerComponent* MCRight = nullptr;

	//The static mesh components attached to both hands for visual representation
	UPROPERTY(EditDefaultsOnly, Category = "VR")
	class UStaticMeshComponent* SMLeft = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "VR")
	UStaticMeshComponent* SMRight = nullptr;

	//The scene components that are responsible for holding a picked up object
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VR")
	USceneComponent* SCHeldObjectLeft = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "VR")
	USceneComponent* SCHeldObjectRight = nullptr;

	//The starting hand model when it's not occupied by an object such as a gun or ammo clip
	UPROPERTY(EditDefaultsOnly, Category = "VR")
	UStaticMesh* EmptyHandModel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	UStaticMesh* GunHoldingModel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	UStaticMesh* FingerPointHandModel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	UStaticMesh* AmmoClipHandModel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	TSubclassOf<class AAmmoPouch> AmmoPouchTemplate = nullptr;

	AAmmoPouch* SpawnedAmmoPouch = nullptr;

	//Components for watches and their UI
	UPROPERTY(EditDefaultsOnly, Category = "VR")
	UStaticMeshComponent* SMLeftWatch = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	UStaticMeshComponent* SMRightWatch = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	class UWidgetComponent* SMLeftWatchTimeWidget = nullptr;

	//TODO: Add a life widget on the right watch

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	class UChildActorComponent* UIDisplay = nullptr;

	//The gun added by default instead of picking it up
	UPROPERTY(EditDefaultsOnly, Category = "Object")
	TSubclassOf<class APlayersGun> PlayersGunTemplate = nullptr;

	APlayersGun* PlayersGun = nullptr;


private:

	bool bIsHoldingObjectLeft = false;
	bool bIsHoldingObjectRight = false;

	//The object we picked up in either hand
	AActor* LeftPickedUpActor = nullptr;
	AActor* RightPickedUpActor = nullptr;

	//The component we actually hit
	UPrimitiveComponent* LeftHitComponent = nullptr;
	UPrimitiveComponent* RightHitComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	float PickUpRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	float HandSwapRange = 20.0f;

	EHand CurrentHandPressed;

	//Used to track how many things are overlapping. If the hand leaves the keypad it should go back to normal
	int32 AmountOfObjectsOverlapping = 0;

	//Health related values
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	int32 StartingLives = 3;

	int32 CurrentLives = 3;

	int32 TotalTimesHit = 0;

	//The time spent hovering over a certain object
	float TimeSpendHoveredOverOption = 0.0f;

	//Methods

public:
	AVRPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Methods for the event manager to trigger for the alarm
	UFUNCTION(BlueprintImplementableEvent, Category = "Section 1 Alarm Effect")
	void TurnAlarmOnOrOff(bool bTurnAlarmOn);

	//TODO: Implement this later!
	void TookDamage();

	//Getters
	class UUserWidget* GetWatchClass();

	int32 GetTotalTimesHit();

	APlayersGun* GetPlayersGun();

protected:
	virtual void BeginPlay() override;
	
private:

	//Interactions
	UFUNCTION()
	void PickUpObjectLeft();

	UFUNCTION()
	void PickUpObjectRight();

	UFUNCTION()
	void DropObjectLeft();

	UFUNCTION()
	void DropObjectRight();

	void TogglePointingHandMeshLeft(float AxisValue);
	void TogglePointingHandMeshRight(float AxisValue);

	//Allows both the left and right hand logic for the ammo clip to be in one function
	UFUNCTION()
	void AttemptToPickUpAmmoClip(EHand HandType);

	//Called for input to the pickup if it's the pistol and not ammo
	void FirePistolLeft();

	void FirePistolRight();

	UFUNCTION()
	void SpawnPistolAndPlaceInRightHand();

	void CreateAmmoPouch();

	//Tutorial Logic
	//Proceeds the tutorial UI to the next one via a delegate
	void ProceedTutorialScreen();

	//Call this from tick if we are at the right stage of the tutorial
	void TutorialGunHandSearch(float DeltaTime);

	//Used to pick which hand to put the gun into when chosen
	void TutorialGunSpawn(EHand HandType);

};
