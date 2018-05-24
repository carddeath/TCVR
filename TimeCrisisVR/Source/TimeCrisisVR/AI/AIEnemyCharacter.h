// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/AIAnimInstance.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIEnemyCharacter.generated.h"

UENUM()
enum class EnemyType : uint8
{
	BLUE,
	BROWN,
	RED,
	ORANGE,
	GREEN
};

UENUM()
enum class EnemyWeapon : uint8
{
	PISTOL,
	MINIGUN,
	ROCKET_LAUNCHER,
	SHIELD_PISTOL
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathCallback, AAIEnemyCharacter*, Char);

UCLASS()
class TIMECRISISVR_API AAIEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	//Variables
public:

	FDeathCallback DeathCallback;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		TArray<class USoundBase*> DeathNoises;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		TArray<USoundBase*> PistolShots;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
		USoundBase* PistolMiss = nullptr;

	//The materials will be different for each enemy. Blue, Brown, Red, Orange, Green etc
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Components")
	TArray<class UMaterialInstance*> EnemyMaterials;

private:

	int32 PercentageOfCriticalHit = 0;

	float DelayToMoveToFurtherPoint;

	EnemyType MyType;
	EnemyWeapon MyWeapon;
	//Where the unit spawns in the world
	class ATargetPoint* SpawnPoint = nullptr;

	//Where the unit should go to, if it's not provided then the spawn point is the correct point
	ATargetPoint* GoToPoint = nullptr;

	//If the character moves after attacking
	ATargetPoint* FurtherGoToPoint = nullptr;

	//Stops the death noise being played mid flight
	bool bIsDead = false;
	//Used to check if we should move to rotate towards the player
	bool bIsAtGoToLocation = false;

	//Methods

public:
	// Sets default values for this character's properties
	AAIEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Used to setup enemies that move from their spawn point
	void SetupEnemy(EnemyType typeOfEnemy, EnemyWeapon EnemyWep, ATargetPoint* SpwnPoint, ATargetPoint* GoToPnt, ATargetPoint* FurtherMovPnt, float DelayToGoToFurtherPoint);

	void KillEnemy(HitArea HitBoxTarget);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	

	void TextureEnemy();

	void SetupEnemyPosition();


	
	
};
