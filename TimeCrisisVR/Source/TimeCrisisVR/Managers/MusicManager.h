// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicManager.generated.h"

UCLASS()
class TIMECRISISVR_API AMusicManager : public AActor
{
	GENERATED_BODY()

//variables
public:
	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	class USoundBase* MainThemeMusic = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* EndOfLevelMusic = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SFX")
	class UAudioComponent* AudioPlayer = nullptr;

private:

	class ANavigationManager* NavManager = nullptr;
	

	//Methods

public:	
	AMusicManager();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:	
	UFUNCTION()
	void StopMainThemeAndPlayEndMusic(int32 junk);

	
	
};
