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

	//Start the game off with the first substage being called directly
	UpdateSubStage(0);


}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::UpdateSubStage(int32 NewSubStage) 
{
	CurrentSubStage = NewSubStage;
	TotalEnemiesShot = 0;
	if (CheckNoFieldsAreEmpty()) 
	{
		PlaceEnemies();
	}

}

bool AEnemySpawner::CheckNoFieldsAreEmpty() 
{
	if (!CharacterToSpawn || EnemyQuantityPerStage1Act1Section1.Num() <= 0 || Stage1Act1Section1Sub0SpawnPoints.Num() <= 0) 
	{
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
	//Check if all enemies are dead for the section
	if (TotalEnemiesShot >= EnemyQuantityPerStage1Act1Section1[CurrentSubStage])
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
	switch (CurrentSubStage)
	{
	case 0:
		PlaceEnemiesStage1Area1SubSection0();
		break;
	case 1:
		PlaceEnemiesStage1Area1SubSection1();
		break;
		//TODO: Do this for each sub sction
	default:
		break;
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1SubSection0() 
{
	//TODO: THIS LOGIC SHOULD BE HANDLED IN THE ENEMY ITSELF BY SENDING THE POINT DATA ACROSS for spawning and move to if required.
	for (int32 i = 0; i < EnemyQuantityStage1Act1Section1Sub0Wave1; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		//Make a brown uniform for the middle soldier
		if (i == 1)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, Stage1Act1Section1Sub0SpawnPoints[i], nullptr, nullptr, 0.0f);
		}
		else
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, Stage1Act1Section1Sub0SpawnPoints[i], nullptr, nullptr, 0.0f);
		}

		//Add the delegate to say when an enemy is dead
		CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1SubSection1() 
{
	for (int32 i = 0; i < EnemyQuantityStage1Act1Section1Sub1Waves[CurrentWaveInSubSection]; i++) 
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (i == 0)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, Stage1Act1Section1Sub1SpawnPoints[i], nullptr, Stage1Act1Section1Sub1FurtherGoToPoints[i], 2.0f);
		}
		else if(i == 1)
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, Stage1Act1Section1Sub1SpawnPoints[i], nullptr, Stage1Act1Section1Sub1FurtherGoToPoints[i], 3.0f);
		}
		else 
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, Stage1Act1Section1Sub1SpawnPoints[i], Stage1Act1Section1Sub1GoToPoints[i], nullptr, 0.0f);
		}

		//Add the delegate to say when an enemy is dead
		CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
	}
}

