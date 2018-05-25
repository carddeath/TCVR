// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPouch.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"

#include "Player/AmmoClip.h"


// Sets default values
AAmmoPouch::AAmmoPouch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(FName("Holster Offset"));

	AmmoPouchOffset = CreateDefaultSubobject<USceneComponent>(FName("HolsterOffset"));

	AmmoPouchOffset->SetupAttachment(RootComp);

	LeftPouch = CreateDefaultSubobject<UStaticMeshComponent>(FName("Left Pouch"));
	RightPouch = CreateDefaultSubobject<UStaticMeshComponent>(FName("Right Pouch"));

	LeftPouch->SetupAttachment(AmmoPouchOffset);
	RightPouch->SetupAttachment(AmmoPouchOffset);


}

// Called when the game starts or when spawned
void AAmmoPouch::BeginPlay()
{
	Super::BeginPlay();

	LeftPouch->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPouch::OnComponentBeginOverlap);
	LeftPouch->OnComponentEndOverlap.AddDynamic(this, &AAmmoPouch::OnComponentEndOverlap);
	RightPouch->OnComponentBeginOverlap.AddDynamic(this, &AAmmoPouch::OnComponentBeginOverlap);
	RightPouch->OnComponentEndOverlap.AddDynamic(this, &AAmmoPouch::OnComponentEndOverlap);
}

// Called every frame
void AAmmoPouch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoPouch::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//These names are taken from the VRPawn static mesh hand objects
	if (OtherComp->GetName() == "SMLeft") 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, TEXT("Collided Left"));
		bPlayersLeftHandInPouch = true;
	}
	else if (OtherComp->GetName() == "SMRight") 
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, TEXT("Collided Right"));
		bPlayersRightHandInPouch = true;
	}
}

void AAmmoPouch::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//These names are taken from the VRPawn static mesh hand objects
	if (OtherComp->GetName() == "SMLeft")
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, TEXT("Removed Left"));
		bPlayersLeftHandInPouch = false;
	}
	else if (OtherComp->GetName() == "SMRight")
	{
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, TEXT("Removed Right"));
		bPlayersRightHandInPouch = false;
	}
}

//Getters

bool AAmmoPouch::GetLeftHandInPouchState()
{
	return bPlayersLeftHandInPouch;
}

bool AAmmoPouch::GetRightHandInPouchState()
{
	return bPlayersRightHandInPouch;
}

TSubclassOf<AAmmoClip> AAmmoPouch::GetClipStaticMesh()
{
	return PistolAmmoClip;
}

