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

	//Used to say how many enemies are in each section in area1. This should add up to the total in the waves
	UPROPERTY(EditDefaultsOnly, Category = "Stage 1 | Area 1 | Setup")
	TArray<int32> TotalEnemiesPerStage1Area1;

	//Section 1
	//---------
	//The target points for where the first units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section1")
	TArray<class ATargetPoint*> Stage1Area1Section1SpawnPoints;

	//---------------------------------------------------------------------

	//Section 2
	//---------
	//The target points for where the units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section2")
		TArray<class ATargetPoint*> Stage1Area1Section2SpawnPoints;

	//The target points where units should run to.
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section2")
		TArray<class ATargetPoint*> Stage1Area1Section2GoToPoints;

	//The target points for when a unit should try to escape the scene
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section2")
		TArray<class ATargetPoint*> Stage1Area1Section2EscapePoints;

	//The target points for when a unit advances after attacking
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section2")
		TArray<class ATargetPoint*> Stage1Area1Section2AdvancePoints;

	//The enemy quantities to be spawned in a sub section. Need an array as we have multiple "waves" of enemies
	UPROPERTY(EditDefaultsOnly, Category = "Stage 1 | Area 1 | Section 2")
		TArray<int32> EnemyQuantityTotalStage1Area1Section2Waves;

	//Section 3
	//---------
	//The target points for where the units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section 3")
		TArray<class ATargetPoint*> Stage1Area1Section3SpawnPoints;

	//The target points where units should run to.
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section 3")
		TArray<class ATargetPoint*> Stage1Area1Section3GoToPoints;

	//The enemy quantities to be spawned in a sub section. Need an array as we have multiple "waves" of enemies
	UPROPERTY(EditDefaultsOnly, Category = "Stage 1 | Area 1 | Section 3")
		TArray<int32> EnemyQuantityTotalStage1Area1Section3Waves;

	//Section 4
	//---------
	//The target points for where the units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section 4")
		TArray<class ATargetPoint*> Stage1Area1Section4SpawnPoints;

	//The target points where units should run to.
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section 4")
		TArray<class ATargetPoint*> Stage1Area1Section4GoToPoints;

	//The target points for when a unit should try to escape the scene
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section 4")
		TArray<class ATargetPoint*> Stage1Area1Section4EscapePoints;

	//The target points for when a unit advances after attacking 
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section 4")
		TArray<class ATargetPoint*> Stage1Area1Section4AdvancePoints;

	//The enemy quantities to be spawned in a sub section. Need an array as we have multiple "waves" of enemies
	UPROPERTY(EditDefaultsOnly, Category = "Stage 1 | Area 1 | Section 4")
		TArray<int32> EnemyQuantityTotalStage1Area1Section4Waves;

	//Section 5
	//---------
	//The target points for where the units should spawn
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section 5")
		TArray<class ATargetPoint*> Stage1Area1Section5SpawnPoints;

	//The target points where units should run to.
	UPROPERTY(EditInstanceOnly, Category = "Stage1_Area 1_Section 5")
		TArray<class ATargetPoint*> Stage1Area1Section5GoToPoints;

	///////////////////////////////////////////////////

	//Only set this when trying to start the game in a different position
	UPROPERTY(EditInstanceOnly, Category = "Debug")
	int32 CurrentSection = 1; //The current area of shooting

private:

	int32 CurrentEnemiesAliveInSection = 0; // Tracks how many units are left for use of waves and moving on to the next section
	int32 TotalEnemiesShot = 0; //Used to tally all dead units within a section to know when a section is complete by accumulating the waves
	int32 CurrentArea = 1; //The current area within an stage 1 -3
	int32 CurrentWaveInSection = 1; //Used to keep track of which "wave of enemies" are in play in a certain area

	TArray<AAIEnemyCharacter*> CurrentEnemiesAlive;

	//Used to toggle in a 
	bool bDoesSectorContainWaves = false;

	//Methods
	
public:	
	AEnemySpawner();
	virtual void Tick(float DeltaTime) override;

	//void UpdateStage(int32 NewStage);
	//void UpdateArea(int32 NewArea);
	void UpdateSection(int32 NewSection);

	UFUNCTION()
	void DecreaseEnemyCount(AAIEnemyCharacter* Char);

	//Debug function
	UFUNCTION(BlueprintCallable, Category = "Debug")
	void DEBUGDeleteAllEnemiesAndAdvanceStage();

protected:
	virtual void BeginPlay() override;

private:	

	void PlaceEnemies();

	//Stage 1 | Area 1 |
	void PlaceEnemiesStage1Area1();
	void PlaceEnemiesStage1Area1Section2();
	void PlaceEnemiesStage1Area1Section3();
	void PlaceEnemiesStage1Area1Section4();

	//Stage 1 | Area 1 | Section 1
	void PlaceEnemiesStage1Area1Section1();

	//Stage 1 | Area 1 | Section 2
	void PlaceEnemiesStage1Area1Section2Wave1();
	void PlaceEnemiesStage1Area1Section2Wave2();
	void PlaceEnemiesStage1Area1Section2Wave3();
	void PlaceEnemiesStage1Area1Section2Wave4();
	void PlaceEnemiesStage1Area1Section2Wave5();
	void PlaceEnemiesStage1Area1Section2Wave6();

	//Stage 1 | Area 1 | Section 3
	void PlaceEnemiesStage1Area1Section3Wave1();
	void PlaceEnemiesStage1Area1Section3Wave2();

	void SpawnEnemyAfterDelayStage1Area1Section3Wave2GoToPnt(int32 PlayerIndex);

	//Stage 1 | Area 1 | Section 4
	void PlaceEnemiesStage1Area1Section4Wave1();
	void PlaceEnemiesStage1Area1Section4Wave2();
	void PlaceEnemiesStage1Area1Section4Wave3();
	void PlaceEnemiesStage1Area1Section4Wave4();

	void SpawnEnemyAfterDelayStage1Area1Section4Wave1(int32 PlayerIndex);

	//Stage 1 | Area 1 | Section 5
	void PlaceEnemiesStage1Area1Section5();

	//Saftey
	bool CheckNoFieldsAreEmpty();
	
};
