// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIAnimInstance.generated.h"

UENUM(Blueprintable)
enum class HitArea : uint8
{
	HEAD,
	TORSO,
	LEGS
};

UENUM(Blueprintable)
enum class EnemyWeapon : uint8
{
	PISTOL,
	ROCKET_LAUNCHER,
	SHIELD_PISTOL,
	PROJECTILE
};

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	//Variables
public:

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsDead = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsFiring = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsReloading = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsRunning = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsCrouching = false;

	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	float speed = 10.0f;

	//Used to decide what death plays
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	HitArea HitAreaSection;

	//Used to decide what fire and reload animation plays
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	EnemyWeapon WeaponInUse;


protected:

private:

	//Methods
public:

protected:

private:
	
	
	
	
};
