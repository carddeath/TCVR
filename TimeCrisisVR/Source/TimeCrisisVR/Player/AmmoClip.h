// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoClip.generated.h"

UCLASS()
class TIMECRISISVR_API AAmmoClip : public AActor
{
	GENERATED_BODY()

		//Variables

public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	int32 ClipAmmo = 6;

private:

	//Methods
	
public:	
	// Sets default values for this actor's properties
	AAmmoClip();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int32 GetAmmo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	
	
};
