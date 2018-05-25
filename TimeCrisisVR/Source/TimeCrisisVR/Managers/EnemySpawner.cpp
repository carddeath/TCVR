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
			RevealNextPointToNavMang.Broadcast(0);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Next point not bound in %s"), *this->GetName());
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
		PlaceEnemiesStage1Area1Section2();
		break;
		//TODO: Do this for each sub section
	default:
		break;
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section1() 
{
	//TODO: THIS LOGIC SHOULD BE HANDLED IN THE ENEMY ITSELF BY SENDING THE POINT DATA ACROSS for spawning and move to if required.
	for (int32 i = 0; i < TotalEnemiesPerStage1Area1[0]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		//Make a brown uniform for the middle soldier
		if (i == 1)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, Stage1Area1Section1SpawnPoints[i], nullptr, nullptr, 0.0f);
		}
		else
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, Stage1Area1Section1SpawnPoints[i], nullptr, nullptr, 0.0f);
		}

		//Add the delegate to say when an enemy is dead
		CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2() 
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section2Waves[CurrentWaveInSubSection]; i++) 
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (i == 0)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, Stage1Area1Section2SpawnPoints[i], nullptr, Stage1Area1Section2FurtherGoToPoints[i], 2.0f);
		}
		else if(i == 1)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, Stage1Area1Section2SpawnPoints[i], nullptr, Stage1Area1Section2FurtherGoToPoints[i], 3.0f);
		}
		else 
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, Stage1Area1Section2SpawnPoints[i], Stage1Area1Section2GoToPoints[i], nullptr, 0.0f);
		}

		//Add the delegate to say when an enemy is dead
		CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
	}
}

