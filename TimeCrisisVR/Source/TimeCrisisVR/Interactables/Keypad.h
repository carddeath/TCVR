// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Keypad.generated.h"

UCLASS()
class TIMECRISISVR_API AKeypad : public AActor
{
	GENERATED_BODY()
	
		//Variables
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* KeypadMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UBoxComponent* KeypadMeshChangingTrigger = nullptr;

private:

	//Methods
public:
	AKeypad();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:


	
	
};
