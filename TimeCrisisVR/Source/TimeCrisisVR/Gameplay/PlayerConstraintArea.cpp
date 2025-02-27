// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerConstraintArea.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
APlayerConstraintArea::APlayerConstraintArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(FName("Root Comp"));
	RootComponent = RootComp;

	AnnotatedArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Annotation Mesh"));
	AnnotatedArrowMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlayerConstraintArea::BeginPlay()
{
	Super::BeginPlay();
	ShowTeleportPad(false);
	
}

// Called every frame
void APlayerConstraintArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerConstraintArea::ShowTeleportPad(bool bShouldShow)
{
	//Enables the arrow to be toggled/along with collision and tick
	this->SetActorHiddenInGame(!bShouldShow);
	this->SetActorEnableCollision(bShouldShow);
	this->SetActorTickEnabled(bShouldShow);
}

void APlayerConstraintArea::ShowTeleportPadAnnotation(bool bShouldShow)
{
	//Enables the arrow to be toggled/along with collision and tick
	AnnotatedArrowMesh->SetHiddenInGame(!bShouldShow);
	//AnnotatedArrowMesh->SetActorEnableCollision(bShouldShow);
	//AnnotatedArrowMesh->SetActorTickEnabled(bShouldShow);
}

