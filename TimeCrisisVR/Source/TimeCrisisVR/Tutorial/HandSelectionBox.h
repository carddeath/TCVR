// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandSelectionBox.generated.h"

UCLASS()
class TIMECRISISVR_API AHandSelectionBox : public AActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

private:

	//M<ethods
	
public:	
	AHandSelectionBox();
	virtual void Tick(float DeltaTime) override;

	void ShowBox(bool bShouldShow);

	UFUNCTION(BlueprintImplementableEvent, Category = "Stare Visuals")
		void ShowTheBoxFilling();

	UFUNCTION(BlueprintImplementableEvent, Category = "Stare Visuals")
		void ResetBoxPosition();
protected:

	virtual void BeginPlay() override;

private:	


	
	
};
