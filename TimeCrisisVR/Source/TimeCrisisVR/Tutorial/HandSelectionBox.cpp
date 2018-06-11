// Fill out your copyright notice in the Description page of Project Settings.

#include "HandSelectionBox.h"


// Sets default values
AHandSelectionBox::AHandSelectionBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHandSelectionBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHandSelectionBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHandSelectionBox::ShowBox(bool bShouldShow)
{
	//Enables the arrow to be toggled/along with collision and tick
	this->SetActorHiddenInGame(!bShouldShow);
	this->SetActorEnableCollision(bShouldShow);
	this->SetActorTickEnabled(bShouldShow);
}

