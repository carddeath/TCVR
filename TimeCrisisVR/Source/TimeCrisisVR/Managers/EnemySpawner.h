// Fill out your copyright notice in the Description page of Project Settings.

/*
Enemy spawner should have different stages and the different types of enemies to spawn
Should communicate with the Navigation Manager with a delegate, when all of the enemies are dead in a section it should tell the navigation manager
The navigation manager should then show the next movement point, when the movement point is active, it should tell the enemy spawner to spawn the next set of enemies.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRevealNextPoint, int, junk);

UCLASS()
class TIMECRISISVR_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

	//Variables
public:

	FRevealNextPoint RevealNextPointToNavMang;

protected:

	//The character mesh to spawn 
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Setup")
	TSubclassOf<class AAIEnemyCharacter> CharacterToSpawn = nullptr;

	//Used to say how many enemies are in each section. This should add up to the total in the waves
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Setup")
	TArray<int32> EnemyQuantityPerStage1Act1Section1;

	//Sub Section 0
	//The target points for where the first units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Act1_Section1_Sub0")
	TArray<class ATargetPoint*> Stage1Act1Section1Sub0SpawnPoints;

	//The enemy quantities to be spawned in a sub section. Only 1 wave so we say 3 and no array
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Setup")
	int32 EnemyQuantityStage1Act1Section1Sub0Wave1 = 3;

	//Sub Section 1
	//The target points for where the first units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Act1_Section1_Sub1")
		TArray<class ATargetPoint*> Stage1Act1Section1Sub1SpawnPoints;

	//The target points for where the first units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Act1_Section1_Sub1")
		TArray<class ATargetPoint*> Stage1Act1Section1Sub1GoToPoints;

	//The target points for where the first units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Act1_Section1_Sub1")
		TArray<class ATargetPoint*> Stage1Act1Section1Sub1FurtherGoToPoints;

	//The enemy quantities to be spawned in a sub section. Need an array as we have multiple "waves" of enemies
	UPROPERTY(EditDefaultsOnly, Category = "Enemy Setup")
		TArray<int32> EnemyQuantityStage1Act1Section1Sub1Waves;

	int32 CurrentWaveInSubSection = 0; //Used to keep track of which "wave of enemies" are in play in a certain area


private:

	int32 CurrentEnemiesAliveInSection = 0;
	int32 TotalEnemiesShot = 0;
	int32 CurrentSubStage = 0; //The current area of shooting
	int32 CurrentArea = 1; //The current area within an stage

	TArray<AAIEnemyCharacter*> CurrentEnemiesAlive;

	//Methods
	
public:	
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;

	//Responsible for getting the new position of the player. Most likely will only use the UpdateSubStage
	//void UpdateLevel(int32 NewLevel);

	//void UpdateAct(int32 NewAct);

	//void UpdateStage(int32 NewStage);

	void UpdateSubStage(int32 NewSubStage);

	UFUNCTION()
	void DecreaseEnemyCount(AAIEnemyCharacter* Char);

protected:
	virtual void BeginPlay() override;

private:	

	void PlaceEnemies();
	void PlaceEnemiesStage1Area1();
	void PlaceEnemiesStage1Area1SubSection0();
	void PlaceEnemiesStage1Area1SubSection1();

	//Saftey
	bool CheckNoFieldsAreEmpty();
	
};
