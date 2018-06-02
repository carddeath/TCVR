// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBox.generated.h"

UCLASS()
class TIMECRISISVR_API AExplosiveBox : public AActor
{
	GENERATED_BODY()

	//Variables
public:

protected:
	UPROPERTY(EditAnywhere, Category = "Data")
		int32 AmtOfShotsRequiredToExplode = 3;

private:

	//Methods
	
public:	
	AExplosiveBox();
	virtual void Tick(float DeltaTime) override;

	//Called from the Player gun
	void TookDamageFromPlayer();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Visuals")
		void FlashBoxWhite();

	UFUNCTION(BlueprintImplementableEvent, Category = "Visuals")
		void ExplodeBox();

private:	


	
};
