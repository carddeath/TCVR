// Fill out your copyright notice in the Description page of Project Settings.

#include "KeypadButton.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "Components/BoxComponent.h"


// Sets default values
AKeypadButton::AKeypadButton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SM_Key_Mesh"));
	PressCollider = CreateDefaultSubobject<UBoxComponent>(FName("Pressing Collision"));
}

// Called when the game starts or when spawned
void AKeypadButton::BeginPlay()
{
	Super::BeginPlay();
	PickMaterial();
}

// Called every frame
void AKeypadButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKeypadButton::PickMaterial() 
{
	KeyMesh->SetMaterial(0, NumberMaterials[NumberToKey]);
}

