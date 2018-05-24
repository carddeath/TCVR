// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Announcer.generated.h"

UCLASS()
class TIMECRISISVR_API AAnnouncer : public AActor
{
	GENERATED_BODY()

		//Variables

public:



protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
	class USoundBase* SFXReload = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase* SFXDanger = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase* SFXAction = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundBase* SFXNewRecord = nullptr;

private:

		//Methods
	
public:	
	AAnnouncer();
	virtual void Tick(float DeltaTime) override;

	//Delegated
	UFUNCTION()
	void PlayReload();

protected:
	virtual void BeginPlay() override;

private:	




	
	
};
