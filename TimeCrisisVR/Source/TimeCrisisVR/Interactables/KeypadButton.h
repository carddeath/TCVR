// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeypadButton.generated.h"

UCLASS()
class TIMECRISISVR_API AKeypadButton : public AActor
{
	GENERATED_BODY()

	//Variables
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Textures")
	TArray<class UMaterialInstance*> NumberMaterials;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* KeyMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UBoxComponent* PressCollider = nullptr;

	//The number this key should produce 0 = Cancel | 1-9 means their number value.
	UPROPERTY(EditInstanceOnly, Category = "Button Values")
		int32 NumberToKey;

private:

	//Methods
public:
	AKeypadButton();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void PickMaterial();



	
	
};
