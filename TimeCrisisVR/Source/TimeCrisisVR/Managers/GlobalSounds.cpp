// Fill out your copyright notice in the Description page of Project Settings.

#include "GlobalSounds.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"

GlobalSounds::GlobalSounds()
{
//Gets the metal sound effect from the content browser and attempts to load it into the variable
	FSoftObjectPath MetalPath;
	MetalPath.SetPath("/Game/Sounds/Weapons/A_Metal_Canister_Enviroment_Impact");
	MetalBulletImpact = Cast<USoundBase>(MetalPath.TryLoad());
}

GlobalSounds::~GlobalSounds()
{
}
