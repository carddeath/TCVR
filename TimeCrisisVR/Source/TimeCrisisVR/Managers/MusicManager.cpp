// Fill out your copyright notice in the Description page of Project Settings.

#include "MusicManager.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "Managers/NavigationManager.h"
#include "EngineUtils.h"


// Sets default values
AMusicManager::AMusicManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioPlayer = CreateDefaultSubobject<UAudioComponent>(FName("Sound Player"));

}

// Called when the game starts or when spawned
void AMusicManager::BeginPlay()
{
	Super::BeginPlay();

	if(MainThemeMusic)
	{
		AudioPlayer->SetSound(MainThemeMusic);
		AudioPlayer->Play();
	}

	for (TActorIterator<ANavigationManager> ita(GetWorld()); ita; ++ita)
	{
		NavManager = *ita;
	}

	if (NavManager) 
	{
		NavManager->EndOfStageAudioToggle.AddDynamic(this, &AMusicManager::StopMainThemeAndPlayEndMusic);
	}
}

// Called every frame
void AMusicManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMusicManager::StopMainThemeAndPlayEndMusic(int32 junk) 
{
	if (EndOfLevelMusic) 
	{
		AudioPlayer->SetSound(EndOfLevelMusic);
		AudioPlayer->Play();
	}
}

