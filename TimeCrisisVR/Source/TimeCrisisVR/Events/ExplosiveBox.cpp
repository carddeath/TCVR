// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveBox.h"
#include "Events/ArmyJeep.h"


// Sets default values
AExplosiveBox::AExplosiveBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExplosiveBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExplosiveBox::TookDamageFromPlayer() 
{
	AmtOfShotsRequiredToExplode--;

	//TODO: Make the texture flash white quickly upon being hit
	UE_LOG(LogTemp, Warning, TEXT("Lives left: %d"), AmtOfShotsRequiredToExplode);

	if (AmtOfShotsRequiredToExplode <= 0) 
	{

		ExplodeBox();
		Cast<AArmyJeep>(GetParentActor())->StartExplodedAnimation();
	}
	else 
	{
		FlashBoxWhite();
	}
}