// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialManager.generated.h"

UCLASS()
class TIMECRISISVR_API ATutorialManager : public AActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UWidgetComponent* TutorialWidget = nullptr;

private:
	class AVRPawn* PlayerCharacter = nullptr;

	//Methods
public:	
	ATutorialManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	

	void AssignDelegates();

	//Proceeds the tutorial step
	UFUNCTION()
	void ProceedTutorialStep(int junk);
};
