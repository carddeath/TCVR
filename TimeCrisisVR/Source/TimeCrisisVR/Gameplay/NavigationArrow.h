// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "NavigationArrow.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API ANavigationArrow : public AStaticMeshActor
{
	GENERATED_BODY()

	//Variables
public:



protected:

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
		UStaticMesh* ArrowMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Meshes")
		UStaticMesh* OrbMesh;

private:

	//If the object is visible
	bool bIsVisible = false;

	//Almost all arrows will not be a tutorial arrow but we need this check
	UPROPERTY(EditInstanceOnly, Category = "Tutorial")
	bool bIsTutorialArrow = false;

	//Methods

public:

	void ShowArrow(bool bShouldShow);
	
	void SetArrowMeshes(bool bIsAnnotatedRotation);

	bool GetTutorialArrowState();

protected:
	virtual void BeginPlay() override;

private:
	
	
	
	
};
