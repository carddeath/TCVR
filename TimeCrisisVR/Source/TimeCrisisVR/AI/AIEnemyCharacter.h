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

//Moved Enemy weapon into AIAnimInstance as this class inherits from it
UENUM()
enum class EAIBehaviour : uint8 
{
	SPAWN_SHOOT,
	SPAWN_RUN_SHOOT_ESCAPE,
	SPAWN_POP_SHOOT_ADVANCE_SHOOT,
	SPAWN_RUN_SHOOT,
	SPAWN_POP_SHOOT,
	SPAWN_POP_SHOOT_ESCAPE
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathCallback, AAIEnemyCharacter*, Char);

UCLASS()
class TIMECRISISVR_API AAIEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	//Variables
public:

	FDeathCallback DeathCallback;

	bool bIsReloading = false;

	//Used for tracking data of total enemies killed against shots
	bool bWasKilledByExplosion = false;

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

	UPROPERTY(BlueprintReadWrite, Category = "AI")
	float ReloadDelay;

private:

	int32 PercentageOfCriticalHit = 0;

	EnemyType MyType;

	EnemyWeapon MyWeapon;

	EAIBehaviour MyBehaviour;
	//Where the unit spawns in the world
	class ATargetPoint* SpawnPoint = nullptr;

	//Where the unit should go to, if it's not provided then the spawn point is the correct point
	ATargetPoint* GoToPoint = nullptr;

	//If the character moves after attacking
	ATargetPoint* EscapePoint = nullptr;

	//A point for when AI advances on a position
	ATargetPoint* AdvancePoint = nullptr;

	//Stops the death noise being played mid flight
	bool bIsDead = false;
	//Used to check if we should move to rotate towards the player
	bool bIsAtGoToLocation = false;


	FTimerHandle ShotDelayHandle;
	FTimerHandle CrouchInvisibleDelayHandle;
	FTimerHandle DeathVanishHandle;

	//Methods
public:
	// Sets default values for this character's properties
	AAIEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Used to setup enemies that move from their spawn point
	void SetupEnemy(EnemyType typeOfEnemy, EnemyWeapon EnemyWep, EAIBehaviour Behaviour, ATargetPoint* SpwnPoint, ATargetPoint* GoToPnt, ATargetPoint* EscPoint, ATargetPoint* AdvancePnt);

	void KillEnemy(HitArea HitBoxTarget);

	//Called if the AI leaves the world
	UFUNCTION(BlueprintCallable, Category = "AI")
		void EraseEnemy();

	bool GetDeathState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "AI")
	void FireUponPlayer();

private:	

	void TextureEnemy();

	void SetupEnemyPosition();

	void SetupEnemyWeaponAnimationState();

	//Helpers
	void RevealEnemyFromCrouchSpawnAdvance();

	void RevealEnemyFromCrouchSpawnPop();

	//REmove this later once weapon classes are created
	void SetupReloadTime();

	//Timer delegate to delete the enemy after death
	void DeleteEnemy();

};
