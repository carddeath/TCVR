// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPouch.generated.h"

UCLASS()
class TIMECRISISVR_API AAmmoPouch : public AActor
{
	GENERATED_BODY()
	
		//Variables

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	class USceneComponent* RootComp = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class USceneComponent* AmmoPouchOffset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* LeftPouch = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* RightPouch = nullptr;

protected:

	//WHAT SHOULD THIS REALLY BE? THINK ABOUT HOW OBJECTS GET ATTACHED IN THE VR PAWN
	UPROPERTY(EditDefaultsOnly, Category = "Spawnable Actors")
	TSubclassOf<class AAmmoClip> PistolAmmoClip = nullptr;

private:

	bool bPlayersLeftHandInPouch = false;
	bool bPlayersRightHandInPouch = false;

	//Methods

public:
	AAmmoPouch();
	virtual void Tick(float DeltaTime) override;

	bool GetLeftHandInPouchState();
	bool GetRightHandInPouchState();
	TSubclassOf<AAmmoClip> GetClipStaticMesh();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	
	
};
