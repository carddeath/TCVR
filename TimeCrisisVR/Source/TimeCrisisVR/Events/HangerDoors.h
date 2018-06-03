// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HangerDoors.generated.h"

UCLASS()
class TIMECRISISVR_API AHangerDoors : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHangerDoors();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
		void ShouldCloseDoors(bool bShouldCloseDoors);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
