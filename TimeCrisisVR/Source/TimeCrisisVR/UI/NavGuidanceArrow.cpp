// Fill out your copyright notice in the Description page of Project Settings.

#include "NavGuidanceArrow.h"
#include "Components/WidgetComponent.h"


// Sets default values
ANavGuidanceArrow::ANavGuidanceArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowWidgetComp = CreateDefaultSubobject<UWidgetComponent>(FName("Arrow Widget Comp"));

}

// Called when the game starts or when spawned
void ANavGuidanceArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANavGuidanceArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

