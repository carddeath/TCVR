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

	//Key components
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USceneComponent* KeyParent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UChildActorComponent* Key1 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key2 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key3 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key4 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key5 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key6 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key7 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key8 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key9 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* KeyCancel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		class UWidgetComponent* NumberDisplay = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* ClearSFX;

private:

	//Methods
public:
	AKeypad();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	
	
};
