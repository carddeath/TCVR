// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

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
	TSubclassOf<class AAmmoPouch> AmmoPouchTemplate = nullptr;

	AAmmoPouch* SpawnedAmmoPouch = nullptr;

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

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	float PickupRotOffsetYaw = 180.0f; //Ideal value is 180

	UPROPERTY(EditDefaultsOnly, Category = "VR")
	float PickupRotOffsetPitch = 90.0f; //Ideal value is 90

	EHand CurrentHandPressed;


	//Methods

public:
	AVRPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	
private:
	UFUNCTION()
	void PickUpObjectLeft();

	UFUNCTION()
	void PickUpObjectRight();

	UFUNCTION()
	void DropObjectLeft();

	UFUNCTION()
	void DropObjectRight();

	//Allows both the left and right hand logic for the ammo clip to be in one function
	UFUNCTION()
	void AttemptToPickUpAmmoClip(EHand HandType);

	//Called for input to the pickup if it's the pistol and not ammo
	void FirePistolLeft();

	void FirePistolRight();

};
