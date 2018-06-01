// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SkeletalMeshActor.h"
#include "Submarine.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API ASubmarine : public ASkeletalMeshActor
{
	GENERATED_BODY()

		//Variables
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UParticleSystemComponent* ExplosionPS = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UParticleSystemComponent* SmokePS = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UParticleSystemComponent* FirePS = nullptr;

private:
	ASubmarine();

	//Methods
public:
	//Simply sets the bool to true in the animation instance, called from the event manager
	void StartSubmarineSinkSequence();

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Audio")
		void PlayExplodingSFX();

private:
	
};
