// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "AI/AIEnemyCharacter.h"
#include "Engine/TargetPoint.h"
#include "Engine.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	CurrentEnemiesAlive.SetNum(10);

	//Start the game off with the first section being called directly
	UpdateSection(CurrentSection);


}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::UpdateSection(int32 NewSection) 
{
	CurrentSection = NewSection;
	TotalEnemiesShot = 0;
	if (CheckNoFieldsAreEmpty()) 
	{
		PlaceEnemies();
	}

}

bool AEnemySpawner::CheckNoFieldsAreEmpty() 
{
	if (!CharacterToSpawn || TotalEnemiesPerStage1Area1.Num() <= 0 || Stage1Area1Section1SpawnPoints.Num() <= 0) 
	{
		UE_LOG(LogTemp, Error, TEXT("Something missing on %s - Please check all defaults and instance"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Something missing on Enemy spawner, please check all values in defaults and instance"));
		return false;
	}
	return true;
}

void AEnemySpawner::DecreaseEnemyCount(AAIEnemyCharacter* Char)
{
	CurrentEnemiesAliveInSection--;
	TotalEnemiesShot++;
	UE_LOG(LogTemp, Warning, TEXT("Amount of soldiers left = %d"), CurrentEnemiesAliveInSection);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Enemy Count is now: %d"), CurrentEnemiesAliveInSection));

	//If we have wiped out all of the enemies in the substage
	//Check if all enemies are dead for the section. We are currently 1 section ahead of the array
	if (TotalEnemiesShot >= TotalEnemiesPerStage1Area1[CurrentSection - 1])
	{
		//Tell the nav manager to activate
		if (RevealNextPointToNavMang.IsBound())
		{
			//Reset the wave as we are moving on to a new section
			CurrentWaveInSection = 0;
			bDoesSectorContainWaves = false; //Reset state incase a sector does not have waves
			RevealNextPointToNavMang.Broadcast(0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Next point not bound in %s"), *this->GetName());
		}

	}
	//If we have waves then we should also check the wave and if all enemies are dead
	else if (bDoesSectorContainWaves) 
	{
		if (CurrentEnemiesAliveInSection == 0) 
		{
			//Then increment the wave and find the correct category of soldiers to load
			CurrentWaveInSection++;
			PlaceEnemiesStage1Area1();
		}
	}
}

//All logic for placing enemies and deciding when to place more depending on the section
//--------------------------------------------------------------------------------------

void AEnemySpawner::PlaceEnemies() 
{
	//Used to decide if we are in area 1/2/3 of each stage. Additional check needed if we add future stages
	switch (CurrentArea) 
	{
	case 1:
		PlaceEnemiesStage1Area1();
		break;
	default:
		break;
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1() 
{
	switch (CurrentSection)
	{
	case 1:
		PlaceEnemiesStage1Area1Section1();
		break;
	case 2:
		bDoesSectorContainWaves = true;
		PlaceEnemiesStage1Area1Section2();
		break;
		//TODO: Do this for each sub section
	default:
		break;
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section1() 
{
	for (int32 i = 0; i < TotalEnemiesPerStage1Area1[0]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		//Make a brown uniform for the middle soldier
		if (i == 1)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_SHOOT,
				Stage1Area1Section1SpawnPoints[i], nullptr, nullptr, nullptr, 0.0f);
		}
		else
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_SHOOT,
				Stage1Area1Section1SpawnPoints[i], nullptr, nullptr, nullptr, 0.0f);
		}

		//Add the delegate to say when an enemy is dead
		CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2()
{
	switch (CurrentWaveInSection) 
	{
	case 1:
		PlaceEnemiesStage1Area1Section2Wave1();
		break;
	case 2:
		PlaceEnemiesStage1Area1Section2Wave2();
		break;
	case 3:
		PlaceEnemiesStage1Area1Section2Wave3();
		break;
	case 4:
		PlaceEnemiesStage1Area1Section2Wave4();
		break;
	case 5:
		PlaceEnemiesStage1Area1Section2Wave5();
		break;
	case 6:
		PlaceEnemiesStage1Area1Section2Wave6();
		break;
	default:
		break;
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave1() 
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section2Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (i == 0)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT_ESCAPE,
				Stage1Area1Section2SpawnPoints[i], Stage1Area1Section2GoToPoints[i], Stage1Area1Section2EscapePoints[i], nullptr, 2.0f);
		}
		else if (i == 1)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT_ESCAPE,
				Stage1Area1Section2SpawnPoints[i], Stage1Area1Section2GoToPoints[i], Stage1Area1Section2EscapePoints[i], nullptr, 5.0f);
		}
		else
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
				Stage1Area1Section2SpawnPoints[i], Stage1Area1Section2GoToPoints[i], nullptr, nullptr, 0.0f);
		}

		//Add the delegate to say when an enemy is dead
		CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave2()
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section2Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (i == 0)
		{
			//We need to apply the offset of TotalEnemiesShot to get the right points. Only for array based logic
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::ORANGE, EnemyWeapon::ROCKET_LAUNCHER, EAIBehaviour::SPAWN_POP_SHOOT_ADVANCE_SHOOT,
				Stage1Area1Section2SpawnPoints[i + TotalEnemiesShot], nullptr, nullptr, Stage1Area1Section2AdvancePoints[i + TotalEnemiesShot ], 2.0f);
		}

		//Add the delegate to say when an enemy is dead
		CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave3()
{

}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave4()
{

}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave5()
{

}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave6()
{

}



