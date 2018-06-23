// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerConstraintArea.generated.h"

UCLASS()
class TIMECRISISVR_API APlayerConstraintArea : public AActor
{
	GENERATED_BODY()
	
		//Variables
public:

protected:

private:
	//If the object is visible
	bool bIsVisible = false;

	//Almost all arrows will not be a tutorial arrow but we need this check
	UPROPERTY(EditInstanceOnly, Category = "Tutorial")
		bool bIsTutorialTeleportPad = false;

		//Methods
public:	

	APlayerConstraintArea();
	virtual void Tick(float DeltaTime) override;

	void ShowTeleportPad(bool bShouldShow);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Visuals")
		void ChangePanelColor(bool bWasHovered);

protected:

	virtual void BeginPlay() override;

private:	



	
	
};
