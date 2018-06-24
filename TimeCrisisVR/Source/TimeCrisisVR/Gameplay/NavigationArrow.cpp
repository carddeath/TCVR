// Fill out your copyright notice in the Description page of Project Settings.

#include "NavigationArrow.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

void ANavigationArrow::BeginPlay()
{
	Super::BeginPlay();

	ShowArrow(false);

}

void ANavigationArrow::ShowArrow(bool bShouldShow) 
{
	//Enables the arrow to be toggled/along with collision and tick
	this->SetActorHiddenInGame(!bShouldShow);
	this->SetActorEnableCollision(bShouldShow);
	this->SetActorTickEnabled(bShouldShow);
}

void ANavigationArrow::SetArrowMeshes(bool bIsAnnotatedRotation) 
{
	if (bIsAnnotatedRotation) 
	{
		if (ArrowMesh)
		{
			Cast<UStaticMeshComponent>(GetRootComponent())->SetStaticMesh(ArrowMesh);
		}
	}
	else 
	{
		if (OrbMesh)
		{
			Cast<UStaticMeshComponent>(GetRootComponent())->SetStaticMesh(OrbMesh);
		}
	}
}

bool ANavigationArrow::GetTutorialArrowState() 
{
	return bIsTutorialArrow;
}




