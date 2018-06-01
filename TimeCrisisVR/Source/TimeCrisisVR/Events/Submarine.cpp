// Fill out your copyright notice in the Description page of Project Settings.

#include "Submarine.h"
#include "Events/SubmarineAnim.h"
#include "Particles/ParticleSystemComponent.h"

ASubmarine::ASubmarine() 
{
	ExplosionPS = CreateDefaultSubobject<UParticleSystemComponent>(FName("Explosion Particle System"));
	SmokePS = CreateDefaultSubobject<UParticleSystemComponent>(FName("Smoke Particle System"));
	FirePS = CreateDefaultSubobject<UParticleSystemComponent>(FName("Fire Particle System"));
	ExplosionPS->bAutoActivate = false;
	SmokePS->bAutoActivate = false;
	FirePS->bAutoActivate = false;

	ExplosionPS->SetupAttachment(RootComponent);
	SmokePS->SetupAttachment(RootComponent);
	FirePS->SetupAttachment(RootComponent);
}

void ASubmarine::StartSubmarineSinkSequence() 
{
	Cast<USubmarineAnim>(this->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsSinking = true;

	if (ExplosionPS && SmokePS && FirePS) 
	{
		ExplosionPS->Activate();
		SmokePS->Activate();
		FirePS->Activate();
	}

	PlayExplodingSFX();
}




