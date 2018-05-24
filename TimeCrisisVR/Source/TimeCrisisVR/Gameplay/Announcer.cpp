// Fill out your copyright notice in the Description page of Project Settings.

#include "Announcer.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AAnnouncer::AAnnouncer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("Audio Comp"));

}

// Called when the game starts or when spawned
void AAnnouncer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnnouncer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAnnouncer::PlayReload()
{
	if (!SFXReload) return;

	if (!AudioComponent->IsPlaying()) 
	{
		AudioComponent->SetSound(SFXReload);
		AudioComponent->Play();
	}

	//UGameplayStatics::PlaySound2D(this, SFXReload);
}

