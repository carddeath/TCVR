// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavGuidanceArrow.generated.h"

UCLASS()
class TIMECRISISVR_API ANavGuidanceArrow : public AActor
{
	GENERATED_BODY()

	//Variables
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UWidgetComponent* ArrowWidgetComp = nullptr;
	
		//TODO: Add a widget component and add the arrow to it

	//Methods
public:	
	ANavGuidanceArrow();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:		
};
