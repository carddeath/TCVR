// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Runtime//HeadMountedDisplay/Public/MotionControllerComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/PrimitiveComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"
#include "Components/ChildActorComponent.h"

#include "Player/PlayersGun.h"
#include "Player/AmmoPouch.h"
#include "Player/AmmoClip.h"
#include "Blueprint/UserWidget.h"
#include "UI/PlayerUIAugment.h"

// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCameraRoot = CreateDefaultSubobject<USceneComponent>(FName("VRCameraRoot"));
	VRCamera = CreateDefaultSubobject<UCameraComponent>(FName("VRCamera"));
	VRCamera->SetupAttachment(VRCameraRoot);

	MCLeft = CreateDefaultSubobject<UMotionControllerComponent>(FName("MCLeft"));
	MCRight = CreateDefaultSubobject<UMotionControllerComponent>(FName("MCRight"));

	MCLeft->SetTrackingMotionSource(FName("Left"));
	MCRight->SetTrackingMotionSource(FName("Right"));

	SMLeft = CreateDefaultSubobject<UStaticMeshComponent>(FName("SMLeft"));
	SMRight = CreateDefaultSubobject<UStaticMeshComponent>(FName("SMRight"));
	SMLeft->SetupAttachment(MCLeft);
	SMRight->SetupAttachment(MCRight);

	SCHeldObjectLeft = CreateDefaultSubobject<USceneComponent>(FName("HeldObjectLocationLeft"));
	SCHeldObjectRight = CreateDefaultSubobject<USceneComponent>(FName("HeldObjectLocationRight"));
	SCHeldObjectLeft->SetupAttachment(MCLeft);
	SCHeldObjectRight->SetupAttachment(MCRight);

	SMLeftWatch = CreateDefaultSubobject<UStaticMeshComponent>(FName("SMLeftWatch"));
	SMRightWatch = CreateDefaultSubobject<UStaticMeshComponent>(FName("SMRightWatch"));
	SMLeftWatch->SetupAttachment(SMLeft);
	SMRightWatch->SetupAttachment(SMRight);

	SMLeftWatchTimeWidget = CreateDefaultSubobject<UWidgetComponent>(FName("Watch Timer Widget Comp"));
	SMLeftWatchTimeWidget->SetupAttachment(SMLeftWatch);

	UIDisplay = CreateDefaultSubobject<UChildActorComponent>(FName("UI Display Child Comp"));
	UIDisplay->SetupAttachment(VRCamera);
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	//Set up offsets and models where required
	SMLeft->SetStaticMesh(EmptyHandModel);
	SMRight->SetStaticMesh(EmptyHandModel);

	SCHeldObjectLeft->SetRelativeLocation(FVector(3.6, 0.0, 0.0));
	SCHeldObjectRight->SetRelativeLocation(FVector(3.6, 0.0, 0.0));

	//Spawns the ammo pouch objects
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnedAmmoPouch = GetWorld()->SpawnActor<AAmmoPouch>(AmmoPouchTemplate, GetActorTransform(), SpawnParams);

	FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, false);
	SpawnedAmmoPouch->AttachToActor(this, AttachRules);

	//Create the gun and place it in the hand at the start of the game
	SpawnPistolAndPlaceInRightHand();
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TESTING - MIGHT NEED THIS IF IT'S A TUTORIAL drawing a linetrace from the head
	FHitResult Hit;
	
	DrawDebugLine(GetWorld(), VRCamera->GetComponentLocation(), VRCamera->GetForwardVector() * 1000, FColor::Purple, false, 0.2f, 0, 1.0f);

	if (GetWorld()->LineTraceSingleByChannel(Hit, VRCamera->GetComponentLocation(), VRCamera->GetForwardVector() * 1000, ECollisionChannel::ECC_WorldDynamic))
	{
		
	}

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("PickupLeft", IE_Pressed, this, &AVRPawn::PickUpObjectLeft);
	PlayerInputComponent->BindAction("PickupLeft", IE_Released, this, &AVRPawn::DropObjectLeft);
	PlayerInputComponent->BindAction("PickupRight", IE_Pressed, this, &AVRPawn::PickUpObjectRight);
	PlayerInputComponent->BindAction("PickupRight", IE_Released, this, &AVRPawn::DropObjectRight);
	PlayerInputComponent->BindAction("FirePistolLeft", IE_Pressed, this, &AVRPawn::FirePistolLeft);
	PlayerInputComponent->BindAction("FirePistolRight", IE_Pressed, this, &AVRPawn::FirePistolRight);
	PlayerInputComponent->BindAction("TutorialContinue", IE_Pressed, this, &AVRPawn::ProceedTutorialScreen);

	PlayerInputComponent->BindAxis("PlayerPointingHandLeft", this, &AVRPawn::TogglePointingHandMeshLeft);
	PlayerInputComponent->BindAxis("PlayerPointingHandRight", this, &AVRPawn::TogglePointingHandMeshRight);
}

void AVRPawn::PickUpObjectLeft() 
{
	//UE_LOG(LogTemp, Warning, TEXT(" INLEFTPICK UP HIT LeftHitComponent is %s and RightHitComponent is %s"), *LeftHitComponent->GetName(), *RightHitComponent->GetName());
	if (GEngine) 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Pickup left called"), true, FVector2D(3.0f, 3.0f));
	}

	//We are already holding something, a clip or the gun. So no reason to pick anything up
	if (bIsHoldingObjectLeft) 
	{
		return;
	}
	//If we are inside the ammo pouch then we should pick up an ammo clip and ignore shooting
	//TODO: UPDATE THIS WHEN NEEDED
	else if(SpawnedAmmoPouch->GetLeftHandInPouchState() && LeftPickedUpActor == nullptr)
	{
		AttemptToPickUpAmmoClip(EHand::LEFT);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("Spawned Clip"), true, FVector2D(3.0f, 3.0f));
		return;
	}

	//Logic for shooting if their is no object in the hand or the hand is not in the ammo pouch

	//Getting info about the controller
	FVector LeftMCLoc = MCLeft->GetComponentLocation();
	FVector LeftMCFwdVec = MCLeft->GetForwardVector() * PickUpRange; //Increase range

	//Perform the trace - TODO: This is temporary, will use a channel later for the bag and ammo clip
	FHitResult Hit;
	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECC_PhysicsBody);
	if (GetWorld()->LineTraceSingleByObjectType(Hit, LeftMCLoc, LeftMCLoc + LeftMCFwdVec, Params)) 
	{
		DrawDebugLine(GetWorld(), LeftMCLoc, LeftMCLoc + LeftMCFwdVec, FColor::Green, false, 2.0f, 0, 2.0f);
		LeftHitComponent = Hit.GetComponent();

		//Turn off physics and attach the object to our scene component and set the bool to say we are holding something.
		if (LeftHitComponent->IsSimulatingPhysics()) 
		{
			LeftPickedUpActor = Hit.GetActor();
			LeftHitComponent->SetSimulatePhysics(false);
			bIsHoldingObjectLeft = true;

			LeftPickedUpActor->SetActorRotation(FRotator::ZeroRotator);

			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			LeftPickedUpActor->AttachToComponent(SCHeldObjectLeft, AttachRules);

			//TODO: Before this make sure we have a gun grab mesh, change the mesh, otherwise the socket wont exist... Also think about the clip vs the gun, does it still fit? or another check needed?
			SMLeft->SetStaticMesh(GunHoldingModel);

			FVector Locat = SMLeft->GetSocketLocation(FName("Gun_Position_Left"));

			LeftPickedUpActor->SetActorLocation(Locat);
			LeftPickedUpActor->SetActorRotation(SMLeft->GetSocketRotation(FName("Gun_Position_Left")));

			if (GEngine)
			{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Pickup left Attached"), true, FVector2D(3.0f, 3.0f));
			}
			

			//If it's the gun hide the left ammo ui
			if (Cast<APlayersGun>(LeftPickedUpActor)) 
			{
				Cast<APlayersGun>(LeftPickedUpActor)->DisableGripUI(true);
			}
		}
		//The object might be in the other hand at the moment - Allows for hand swapping
		else 
		{	//If we hit the component in the opposite hand
			//Should be constrained based on distance
			FVector HandSwapVector = MCLeft->GetForwardVector() * HandSwapRange;
			if (LeftHitComponent == RightHitComponent && GetWorld()->LineTraceSingleByObjectType(Hit, LeftMCLoc, LeftMCLoc + HandSwapVector, Params))
			{

				FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
				RightPickedUpActor->DetachFromActor(DetachRules);

				LeftPickedUpActor = RightPickedUpActor;

				LeftPickedUpActor->SetActorRotation(FRotator::ZeroRotator);

				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
				LeftPickedUpActor->AttachToComponent(SCHeldObjectLeft, AttachRules);

				SMLeft->SetStaticMesh(GunHoldingModel);

				FVector Locat = SMLeft->GetSocketLocation(FName("Gun_Position_Left"));

				LeftPickedUpActor->SetActorLocation(Locat);

				LeftPickedUpActor->SetActorRotation(SMLeft->GetSocketRotation(FName("Gun_Position_Left")));

				RightPickedUpActor = nullptr;
				RightHitComponent = nullptr;
				bIsHoldingObjectLeft = true;
				bIsHoldingObjectRight = false;

				//Set the empty hand for the right hand
				SMRight->SetStaticMesh(EmptyHandModel);

				//If it's the gun hide the left ammo ui
				if (Cast<APlayersGun>(LeftPickedUpActor))
				{
					Cast<APlayersGun>(LeftPickedUpActor)->DisableGripUI(true);
				}
			}
		}
	}
	else 
	{
		DrawDebugLine(GetWorld(), LeftMCLoc, LeftMCLoc + LeftMCFwdVec, FColor::Red, false, 5.0f, 0, 2.0f);
	}
}

void AVRPawn::PickUpObjectRight()
{
	if (GEngine)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Pickup right called"), true, FVector2D(3.0f, 3.0f));
	}

	//We are already holding something, a clip or the gun. So no reason to pick anything up
	if (bIsHoldingObjectRight)
	{
		return;
	}
	//If we are inside the ammo pouch then we should pick up an ammo clip and ignore shooting
	//TODO: UPDATE THIS WHEN NEEDED by playing clip in right position
	else if (SpawnedAmmoPouch->GetRightHandInPouchState() && RightPickedUpActor == nullptr )
	{
		AttemptToPickUpAmmoClip(EHand::RIGHT);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("Spawned Clip"), true, FVector2D(3.0f, 3.0f));
		return;
	}

	//Logic for shooting if their is no object in the hand or the hand is not in the ammo pouch
	//Getting info about the controller
	FVector RightMCLoc = MCRight->GetComponentLocation();
	FVector RightMCFwdVec = MCRight->GetForwardVector() * PickUpRange; //Increase range

	//Perform the trace - TODO: This is temporary, will use a channel later for the bag and ammo clip
	FHitResult Hit;
	FCollisionObjectQueryParams Params;
	Params.AddObjectTypesToQuery(ECC_PhysicsBody);
	if (GetWorld()->LineTraceSingleByObjectType(Hit, RightMCLoc, RightMCLoc + RightMCFwdVec, Params))
	{
		DrawDebugLine(GetWorld(), RightMCLoc, RightMCLoc + RightMCFwdVec, FColor::Green, false, 2.0f, 0, 2.0f);
		RightHitComponent = Hit.GetComponent();

		//Turn off physics and attach the object to our scene component and set the bool to say we are holding something.
		if (RightHitComponent->IsSimulatingPhysics())
		{
			RightPickedUpActor = Hit.GetActor();
			RightHitComponent->SetSimulatePhysics(false);
			bIsHoldingObjectRight = true;

			RightPickedUpActor->SetActorRotation(FRotator::ZeroRotator);

			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			RightPickedUpActor->AttachToComponent(SCHeldObjectRight, AttachRules);

			//////////////////////////////////////////////
			//TODO: Before this make sure we have a gun grab mesh, change the mesh, otherwise the socket wont exist... Also think about the clip vs the gun, does it still fit? or another check needed?
			SMRight->SetStaticMesh(GunHoldingModel);

			FVector Locat = SMRight->GetSocketLocation(FName("Gun_Position_Right"));

			RightPickedUpActor->SetActorLocation(Locat);
			RightPickedUpActor->SetActorRotation(SMRight->GetSocketRotation(FName("Gun_Position_Right")));

			if (GEngine)
			{
			//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Pickup right Attached"), true, FVector2D(3.0f, 3.0f));
			}

			//If it's the gun hide the right ammo ui
			if (Cast<APlayersGun>(RightPickedUpActor))
			{
				Cast<APlayersGun>(RightPickedUpActor)->DisableGripUI(false);
			}
		}
		//The object might be in the other hand at the moment - Allows for hand swapping
		else
		{	//If we hit the component in the opposite hand
			//Should be constrained based on distance

			FVector HandSwapVector = MCRight->GetForwardVector() * HandSwapRange;
			if (RightHitComponent == LeftHitComponent && GetWorld()->LineTraceSingleByObjectType(Hit, RightMCLoc, RightMCLoc + HandSwapVector, Params))
			{
				FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
				LeftPickedUpActor->DetachFromActor(DetachRules);

				RightPickedUpActor = LeftPickedUpActor;

				RightPickedUpActor->SetActorRotation(FRotator::ZeroRotator);

				FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
				RightPickedUpActor->AttachToComponent(SCHeldObjectRight, AttachRules);

				SMRight->SetStaticMesh(GunHoldingModel);

				FVector Locat = SMRight->GetSocketLocation(FName("Gun_Position_Right"));

				RightPickedUpActor->SetActorLocation(Locat);

				//	LeftPickedUpActor->AttachToComponent(SMLeft, AttachRules, FName("Gun_Position"));
				RightPickedUpActor->SetActorRotation(SMRight->GetSocketRotation(FName("Gun_Position_Right")));


				LeftPickedUpActor = nullptr;
				LeftHitComponent = nullptr;
				bIsHoldingObjectRight = true;
				bIsHoldingObjectLeft = false;

				//Set the empty hand for the left hand
				SMLeft->SetStaticMesh(EmptyHandModel);

				//If it's the gun hide the right ammo ui
				if (Cast<APlayersGun>(RightPickedUpActor))
				{
					Cast<APlayersGun>(RightPickedUpActor)->DisableGripUI(false);
				}
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), RightMCLoc, RightMCLoc + RightMCFwdVec, FColor::Red, false, 5.0f, 0, 2.0f);
	}
}

//TODO: UPDATE THIS WHEN NEEDED
void AVRPawn::AttemptToPickUpAmmoClip(EHand HandType)
{
	TSubclassOf<AAmmoClip> AmmoClip = SpawnedAmmoPouch->GetClipStaticMesh();
	FActorSpawnParameters SpawnInfo;

	//TODO: Call a function for getting ammo using the ammo pouch class

	if (AmmoClip)
	{
		if (HandType == EHand::LEFT)
		{
			//Set the hand to be the grab gun hand atm
			SMLeft->SetStaticMesh(AmmoClipHandModel);

			//Spawning the ammo clip
			LeftPickedUpActor = GetWorld()->SpawnActor<AAmmoClip>(AmmoClip);

			LeftHitComponent = LeftPickedUpActor->FindComponentByClass<UPrimitiveComponent>();
			if (LeftHitComponent->IsSimulatingPhysics())
			{
				LeftHitComponent->SetSimulatePhysics(false);
				bIsHoldingObjectLeft = true;
			}

			//TODO: Change where the ammo clip goes in the hand
			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			LeftPickedUpActor->AttachToComponent(SCHeldObjectLeft, AttachRules);

			FVector Locat = SMLeft->GetSocketLocation(FName("Ammo_Clip_Left"));

			LeftPickedUpActor->SetActorLocation(Locat);

			//	LeftPickedUpActor->AttachToComponent(SMLeft, AttachRules, FName("Gun_Position"));
			LeftPickedUpActor->SetActorRotation(SMLeft->GetSocketRotation(FName("Ammo_Clip_Left")));

		}
		else if (HandType == EHand::RIGHT) 
		{
			SMRight->SetStaticMesh(AmmoClipHandModel);

			//Spawning the ammo clip
			RightPickedUpActor = GetWorld()->SpawnActor<AAmmoClip>(AmmoClip);

			RightHitComponent = RightPickedUpActor->FindComponentByClass<UPrimitiveComponent>();
			if (RightHitComponent->IsSimulatingPhysics())
			{
				RightHitComponent->SetSimulatePhysics(false);
				bIsHoldingObjectRight = true;
			}

			//TODO: Change where the ammo clip goes in the hand

			FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
			RightPickedUpActor->AttachToComponent(SCHeldObjectRight, AttachRules);

			FVector Locat = SMRight->GetSocketLocation(FName("Ammo_Clip_Right"));

			RightPickedUpActor->SetActorLocation(Locat);

			//	LeftPickedUpActor->AttachToComponent(SMLeft, AttachRules, FName("Gun_Position"));
			RightPickedUpActor->SetActorRotation(SMRight->GetSocketRotation(FName("Ammo_Clip_Right")));
		}
	}
}

//Dropping logic

void AVRPawn::DropObjectLeft()
{
	APlayersGun* PlayerGun = Cast<APlayersGun>(LeftPickedUpActor);

	if (LeftPickedUpActor != nullptr && PlayerGun == nullptr)
	{
		SMLeft->SetStaticMesh(EmptyHandModel);

		//May need be true
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
		LeftPickedUpActor->DetachFromActor(DetachRules);

		if (LeftHitComponent != nullptr)
		{
			LeftHitComponent->SetSimulatePhysics(true);
		}

		bIsHoldingObjectLeft = false;
		LeftHitComponent = nullptr;
		LeftPickedUpActor = nullptr;
		if (GEngine)
		{
		//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
			//	FString::Printf(TEXT("Drop Called and LeftBool is now %s"), bIsHoldingObjectLeft ? TEXT("True") : TEXT("False")), true, FVector2D(3.0f, 3.0f));
		}
	}
}

void AVRPawn::DropObjectRight() 
{
	APlayersGun* PlayerGun = Cast<APlayersGun>(RightPickedUpActor);

	if (RightPickedUpActor != nullptr && PlayerGun == nullptr)
	{
		SMRight->SetStaticMesh(EmptyHandModel);

		//May need be true
		FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
		RightPickedUpActor->DetachFromActor(DetachRules);

		if (RightHitComponent != nullptr)
		{
			RightHitComponent->SetSimulatePhysics(true);
		}

		bIsHoldingObjectRight = false;
		RightHitComponent = nullptr;
		RightPickedUpActor = nullptr;
		if (GEngine)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow,
			//	FString::Printf(TEXT("Drop Called and RightBool is now %s"), bIsHoldingObjectRight ? TEXT("True") : TEXT("False")) , true, FVector2D(3.0f, 3.0f));
		}
	}
}

//For Firing, passes to the object if present
void AVRPawn::FirePistolLeft() 
{
	//tries to cast the gun in both hands
	APlayersGun* PlayersGunReference = nullptr;

	//Make sure that we are holding something.
	if (LeftPickedUpActor != nullptr) 
	{
		PlayersGunReference = Cast<APlayersGun>(LeftPickedUpActor);

		if (PlayersGunReference != nullptr)
		{
			PlayersGunReference->Fire();
		}
	}	
}

//For Firing, passes to the object if present
void AVRPawn::FirePistolRight()
{
	//tries to cast the gun in both hands
	APlayersGun* PlayersGunReference = nullptr;

	//Make sure that we are holding something.
	if (RightPickedUpActor != nullptr)
	{
		PlayersGunReference = Cast<APlayersGun>(RightPickedUpActor);

		if (PlayersGunReference != nullptr)
		{
			PlayersGunReference->Fire();
		}
	}
}

void AVRPawn::TogglePointingHandMeshLeft(float AxisValue) 
{
	if (!bIsHoldingObjectLeft && AxisValue <= 0.0f) 
	{
		SMLeft->SetStaticMesh(FingerPointHandModel);
	}
	else if (!bIsHoldingObjectLeft && AxisValue >= 1.0f) 
	{
		SMLeft->SetStaticMesh(EmptyHandModel);
	}
}

void AVRPawn::TogglePointingHandMeshRight(float AxisValue)
{
	if (!bIsHoldingObjectRight && AxisValue <= 0.0f)
	{
		SMRight->SetStaticMesh(FingerPointHandModel);
	}
	else if (!bIsHoldingObjectRight && AxisValue >= 1.0f)
	{
		SMRight->SetStaticMesh(EmptyHandModel);
	}
}

void AVRPawn::SpawnPistolAndPlaceInRightHand() 
{
	PlayersGun = GetWorld()->SpawnActor<APlayersGun>(PlayersGunTemplate);
	//PlayersGun->FindComponentByClass<UStaticMeshComponent>()->SetRelativeScale3D(FVector(2.0f));
	//PlayersGun->FindComponentByClass<UStaticMeshComponent>()->SetSimulatePhysics(false);

	RightHitComponent = PlayersGun->FindComponentByClass<UStaticMeshComponent>();
	RightHitComponent->SetRelativeScale3D(FVector(2.0f));
	RightHitComponent->SetSimulatePhysics(false);
	RightPickedUpActor = PlayersGun;
	bIsHoldingObjectRight = true;

	RightPickedUpActor->SetActorRotation(FRotator::ZeroRotator);

	FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	RightPickedUpActor->AttachToComponent(SCHeldObjectRight, AttachRules);

	SMRight->SetStaticMesh(GunHoldingModel);

	FVector Locat = SMRight->GetSocketLocation(FName("Gun_Position_Right"));

	RightPickedUpActor->SetActorLocation(Locat);
	RightPickedUpActor->SetActorRotation(SMRight->GetSocketRotation(FName("Gun_Position_Right")));

	//If it's the gun hide the right ammo ui
	if (Cast<APlayersGun>(RightPickedUpActor))
	{
		Cast<APlayersGun>(RightPickedUpActor)->DisableGripUI(false);
	}

	//Lets send this gun to the data tracker
	if (GunWasCreatedDelegate.IsBound()) 
	{
		GunWasCreatedDelegate.Broadcast(PlayersGun);
	}
}

//Tutorial Logic
void AVRPawn::ProceedTutorialScreen() 
{
	//Will not be bound in the main game
	if (TutorialProceedDelegate.IsBound()) 
	{
		TutorialProceedDelegate.Broadcast(0);
	}
}

//Health Related Classes
void AVRPawn::TookDamage() 
{
	//TODO: Remove life total 
	//TODO: SHow the UI change
	TotalTimesHit++;
}

//Getters
UUserWidget* AVRPawn::GetWatchClass() 
{
	return SMLeftWatchTimeWidget->GetUserWidgetObject();
}

int32 AVRPawn::GetTotalTimesHit() 
{
	return TotalTimesHit;
}
