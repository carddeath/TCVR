// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "AI/AIEnemyCharacter.h"
#include "Engine/TargetPoint.h"
#include "Engine.h"
#include "Components/SkeletalMeshComponent.h"

#include "Managers/EventManager.h"


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

	//Find the event manager
	for (TActorIterator<AEventManager> it(GetWorld()); it; ++it) 
	{
		EventMang = *it;

		if (!EventMang) 
		{
			UE_LOG(LogTemp, Error, TEXT("Missing Event manager on %s"), *this->GetName());
		}
	}

	CurrentEnemiesAlive.SetNum(10);

	//if (!bIsTutorial) 
	//{
	//	//Start the game off with the first section being called directly
	//	UpdateSection(CurrentSection);
	//}
}

//Called from the event manager to make sure that we can do the correct behaviour when spawning enemies
void AEnemySpawner::TutorialOrRegularGame(bool bIsRegularGame) 
{
	if (bIsRegularGame) 
	{
		CurrentEnemiesAlive.SetNum(10);
		UpdateSection(CurrentSection);
	}
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
	//TODO: Uncomment this and add all important fields later
	//if (!CharacterToSpawn || TotalEnemiesPerStage1Area1.Num() <= 0 || Stage1Area1Section1SpawnPoints.Num() <= 0) 
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Something missing on %s - Please check all defaults and instance"), *this->GetName());
	//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Something missing on Enemy spawner, please check all values in defaults and instance"));
	//	return false;
	//}
	//return true;
	return true;
}

void AEnemySpawner::DecreaseEnemyCount(AAIEnemyCharacter* Char)
{

	CurrentEnemiesAliveInSection--;
	TotalEnemiesShot++;
	UE_LOG(LogTemp, Warning, TEXT("Amount of soldiers left = %d"), CurrentEnemiesAliveInSection);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, FString::Printf(TEXT("Enemy Count is now: %d"), CurrentEnemiesAliveInSection));

	//If we are in the tutorial and have killed all enemies
	if (bIsTutorial && TotalEnemiesShot >= 3)
	{
		if (TutorialKilledEnemiesDele.IsBound()) 
		{
			TutorialKilledEnemiesDele.Broadcast(0);
			TutorialKilledEnemiesDele.Clear();
		}
	}

	//If we have wiped out all of the enemies in the substage
	//Check if all enemies are dead for the section. We are currently 1 section ahead of the array
	if (TotalEnemiesShot >= TotalEnemiesPerStage1Area1[CurrentSection - 1])
	{

		//Tell the nav manager to activate
		if (RevealNextPointToNavMang.IsBound())
		{
			//Reset the wave as we are moving on to a new section
			CurrentWaveInSection = 1;
			bDoesSectorContainWaves = false; //Reset state incase a sector does not have waves
			RevealNextPointToNavMang.Broadcast(0);
			return;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Next point not bound in %s"), *this->GetName());
		}


	}
	//If we have waves then we should also check the wave and if all enemies are dead
	else if (bDoesSectorContainWaves) 
	{
		if (CurrentEnemiesAliveInSection <= 0) 
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
		bDoesSectorContainWaves = false;
		PlaceEnemiesStage1Area1Section1();
		break;
	case 2:
		bDoesSectorContainWaves = true;
		PlaceEnemiesStage1Area1Section2();
		break;
	case 3:
		bDoesSectorContainWaves = true;
		PlaceEnemiesStage1Area1Section3();
		break;
	case 4:
		bDoesSectorContainWaves = true;
		PlaceEnemiesStage1Area1Section4();
		break;
	case 5:
		bDoesSectorContainWaves = false;
		PlaceEnemiesStage1Area1Section5();
		break;
	default:
		break;
	}
}

//Stage 1 | Area 1 | Section 1
void AEnemySpawner::PlaceEnemiesStage1Area1Section1() 
{
	for (int32 i = 0; i < TotalEnemiesPerStage1Area1[0]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);

		//Allow a delegate to call so decrease the enemy count

		if (CurrentEnemiesAlive[i]) 
		{
			//Make a brown uniform for the middle soldier
			if (i == 1)
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_SHOOT,
					Stage1Area1Section1SpawnPoints[i], nullptr, nullptr, nullptr);
			}
			else
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_SHOOT,
					Stage1Area1Section1SpawnPoints[i], nullptr, nullptr, nullptr);
			}

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}

	}
}


//Stage 1 | Area 1 | Section 2
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

		if (CurrentEnemiesAlive[i]) 
		{
			if (i == 0)
			{

				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT_ESCAPE,
					Stage1Area1Section2SpawnPoints[i], Stage1Area1Section2GoToPoints[i], Stage1Area1Section2EscapePoints[i], nullptr);
			}
			else if (i == 1)
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT_ESCAPE,
					Stage1Area1Section2SpawnPoints[i], Stage1Area1Section2GoToPoints[i], Stage1Area1Section2EscapePoints[i], nullptr);
			}
			else
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
					Stage1Area1Section2SpawnPoints[i], Stage1Area1Section2GoToPoints[i], nullptr, nullptr);
			}

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
		
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

		if (CurrentEnemiesAlive[i]) 
		{

			//We need to apply the offset of TotalEnemiesShot to get the right points. Only for array based logic
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::ORANGE, EnemyWeapon::ROCKET_LAUNCHER, EAIBehaviour::SPAWN_POP_SHOOT_ADVANCE_SHOOT,
				Stage1Area1Section2SpawnPoints[i + TotalEnemiesShot], nullptr, nullptr, Stage1Area1Section2AdvancePoints[i + TotalEnemiesShot]);

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);

			//We want the submarine to be blown up when this soldier dies
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::LaunchRocketSoldierDead);
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave3()
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section2Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (CurrentEnemiesAlive[i])
		{

			//We need to apply the offset of TotalEnemiesShot to get the right points. Only for array based logic
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
				Stage1Area1Section2SpawnPoints[i + TotalEnemiesShot], Stage1Area1Section2GoToPoints[i + TotalEnemiesShot], nullptr, nullptr);

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave4()
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section2Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (CurrentEnemiesAlive[i])
		{

			//We need to apply the offset of TotalEnemiesShot to get the right points. Only for array based logic
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_POP_SHOOT,
				Stage1Area1Section2SpawnPoints[i + TotalEnemiesShot], nullptr, nullptr, nullptr);

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave5()
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section2Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (CurrentEnemiesAlive[i])
		{

			//We need to apply the offset of TotalEnemiesShot to get the right points. Only for array based logic
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
				Stage1Area1Section2SpawnPoints[i + TotalEnemiesShot], Stage1Area1Section2GoToPoints[i + TotalEnemiesShot], nullptr, nullptr);

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section2Wave6()
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section2Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (CurrentEnemiesAlive[i])
		{

			//We need to apply the offset of TotalEnemiesShot to get the right points. Only for array based logic
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
				Stage1Area1Section2SpawnPoints[i + TotalEnemiesShot], Stage1Area1Section2GoToPoints[i + TotalEnemiesShot], nullptr, nullptr);

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

//Stage 1 | Area 1 | Section 3
void AEnemySpawner::PlaceEnemiesStage1Area1Section3() 
{
	switch (CurrentWaveInSection)
	{
	case 1:
		PlaceEnemiesStage1Area1Section3Wave1();
		break;
	case 2:
		PlaceEnemiesStage1Area1Section3Wave2();
		break;
	default:
		break;
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section3Wave1() 
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section3Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (CurrentEnemiesAlive[i])
		{
			if (i == 0)
			{

				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_POP_SHOOT,
					Stage1Area1Section3SpawnPoints[i + TotalEnemiesShot], nullptr, nullptr, nullptr);
			}
			else
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::SHIELD_PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
					Stage1Area1Section3SpawnPoints[i + TotalEnemiesShot], Stage1Area1Section3GoToPoints[i + TotalEnemiesShot], nullptr, nullptr);
			}

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section3Wave2() 
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section3Waves[CurrentWaveInSection - 1]; i++)
	{
	//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		FTimerHandle TempHandle;
		FTimerDelegate SpawnEnemyDelegate = FTimerDelegate::CreateUObject(this, &AEnemySpawner::SpawnEnemyAfterDelayStage1Area1Section3Wave2GoToPnt, i);

		//Need different time handles to create the delay illusion
		switch (i) 
		{
		case 0:
			GetWorldTimerManager().SetTimer(TempHandle, SpawnEnemyDelegate, 1.0f, false);
			break;
		case 1:
			GetWorldTimerManager().SetTimer(TempHandle, SpawnEnemyDelegate, 1.5f, false);
			break;
		case 2:
			GetWorldTimerManager().SetTimer(TempHandle, SpawnEnemyDelegate, 2.25f, false);
			break;
		case 3:
			GetWorldTimerManager().SetTimer(TempHandle, SpawnEnemyDelegate, 2.5f, false);
			break;
		default:
			break;
		}
	}
}

//A delegate method that allows us to spawn soldiers after a delay but with the correct index
void AEnemySpawner::SpawnEnemyAfterDelayStage1Area1Section3Wave2GoToPnt(int32 PlayerIndex)
{
	CurrentEnemiesAlive[PlayerIndex] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);

		if (PlayerIndex == 0 || PlayerIndex == 3)
		{
			CurrentEnemiesAlive[PlayerIndex]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
				Stage1Area1Section3SpawnPoints[PlayerIndex + TotalEnemiesShot], Stage1Area1Section3GoToPoints[PlayerIndex + TotalEnemiesShot], nullptr, nullptr);
		}
		else
		{
			CurrentEnemiesAlive[PlayerIndex]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_POP_SHOOT,
				Stage1Area1Section3SpawnPoints[PlayerIndex + TotalEnemiesShot], nullptr, nullptr, nullptr);
		}

	CurrentEnemiesAlive[PlayerIndex]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
}

//Stage 1 | Area 1 | Section 4
void AEnemySpawner::PlaceEnemiesStage1Area1Section4() 
{
	switch (CurrentWaveInSection) 
	{
	case 1:
		PlaceEnemiesStage1Area1Section4Wave1();
		break;
	case 2:
		PlaceEnemiesStage1Area1Section4Wave2();
		break;
	case 3:
		PlaceEnemiesStage1Area1Section4Wave3();
		break;
	case 4:
		PlaceEnemiesStage1Area1Section4Wave4();
		break;
	default:
		break;
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section4Wave1() 
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section4Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);
		//Allow a delegate to call so decrease the enemy count

		if (CurrentEnemiesAlive[i])
		{
			if (i == 2)
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
					Stage1Area1Section4SpawnPoints[i + TotalEnemiesShot], Stage1Area1Section4GoToPoints[i + TotalEnemiesShot], nullptr, nullptr);
			}
			else if (i == 3) 
			{
				FTimerHandle TempHandle;
				FTimerDelegate SpawnEnemyDelegate = FTimerDelegate::CreateUObject(this, &AEnemySpawner::SpawnEnemyAfterDelayStage1Area1Section4Wave1, i);
				GetWorldTimerManager().SetTimer(TempHandle, SpawnEnemyDelegate, 1.0f, false);
			}
			else
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_SHOOT,
					Stage1Area1Section4SpawnPoints[i + TotalEnemiesShot], nullptr , nullptr, nullptr);
			}

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

void AEnemySpawner::SpawnEnemyAfterDelayStage1Area1Section4Wave1(int32 PlayerIndex) 
{
	//TODO: Change this to the run away one
	CurrentEnemiesAlive[PlayerIndex]->SetupEnemy(EnemyType::ORANGE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_POP_SHOOT_ESCAPE,
		Stage1Area1Section4SpawnPoints[PlayerIndex + TotalEnemiesShot], nullptr, Stage1Area1Section4EscapePoints[PlayerIndex + TotalEnemiesShot], nullptr);
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section4Wave2()
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section4Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);

		if (CurrentEnemiesAlive[i])
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::GREEN, EnemyWeapon::PROJECTILE, EAIBehaviour::SPAWN_POP_SHOOT_ADVANCE_SHOOT,
				Stage1Area1Section4SpawnPoints[i + TotalEnemiesShot], nullptr, nullptr, Stage1Area1Section4AdvancePoints[i + TotalEnemiesShot]);

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section4Wave3()
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section4Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);

		if (CurrentEnemiesAlive[i])
		{
			CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_POP_SHOOT,
				Stage1Area1Section4SpawnPoints[i + TotalEnemiesShot], nullptr, nullptr, nullptr);

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

void AEnemySpawner::PlaceEnemiesStage1Area1Section4Wave4()
{
	for (int32 i = 0; i < EnemyQuantityTotalStage1Area1Section4Waves[CurrentWaveInSection - 1]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);

		if (CurrentEnemiesAlive[i])
		{
			if (i == 0) 
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_POP_SHOOT,
					Stage1Area1Section4SpawnPoints[i + TotalEnemiesShot], nullptr, nullptr, nullptr);
			}
			else 
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::RED, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_POP_SHOOT,
					Stage1Area1Section4SpawnPoints[i + TotalEnemiesShot], nullptr, nullptr, nullptr);
			}


			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}
	}
}

//Stage 1 | Area 1 | Section 5
void AEnemySpawner::PlaceEnemiesStage1Area1Section5() 
{
	for (int32 i = 0; i < TotalEnemiesPerStage1Area1[4]; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);

		//Allow a delegate to call so decrease the enemy count

		if (CurrentEnemiesAlive[i])
		{
			//Make a brown uniform for the middle soldier
			if (i == 0)
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_POP_SHOOT,
					Stage1Area1Section5SpawnPoints[i + TotalEnemiesShot], nullptr , nullptr, nullptr);
			}
			else
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_RUN_SHOOT,
					Stage1Area1Section5SpawnPoints[i + TotalEnemiesShot], Stage1Area1Section5GoToPoints[i + TotalEnemiesShot], nullptr, nullptr);
			}

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}

	}
}

//Debug Methods

//Used in the level manager to "kill" all enemies in the scene and to create the next wave.
void AEnemySpawner::DEBUGDeleteAllEnemiesAndAdvanceStage() 
{
	//BUG: This is broken, not sure why. It creates a new soldier in the 2nd clearence
	//Delete all the enemies in the scene and in turn advance to the next wave 

	if (CurrentSection == 1) 
	{
	
	}
	else if (CurrentSection == 2) 
	{
		TotalEnemiesShot += EnemyQuantityTotalStage1Area1Section2Waves[CurrentWaveInSection - 1];
		CurrentEnemiesAliveInSection = 0;
		DecreaseEnemyCount(nullptr);
	}
}

void AEnemySpawner::SpawnTutorialEnemies() 
{
	//3 because we want to spawn 3 enemies
	for (int32 i = 0; i < 3; i++)
	{
		//Increment how many enemies are alive
		CurrentEnemiesAliveInSection++;

		CurrentEnemiesAlive[i] = GetWorld()->SpawnActor<AAIEnemyCharacter>(CharacterToSpawn);

		if (CurrentEnemiesAlive[i])
		{
			//Make a brown uniform for the middle soldier
			if (i == 1)
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BROWN, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_SHOOT,
					TutorialSpawnPoints[i], nullptr, nullptr, nullptr);
			}
			else
			{
				CurrentEnemiesAlive[i]->SetupEnemy(EnemyType::BLUE, EnemyWeapon::PISTOL, EAIBehaviour::SPAWN_SHOOT,
					TutorialSpawnPoints[i], nullptr, nullptr, nullptr);
			}

			//Add the delegate to say when an enemy is dead
			CurrentEnemiesAlive[i]->DeathCallback.AddDynamic(this, &AEnemySpawner::DecreaseEnemyCount);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Problem with spawning on %s"), *this->GetName());
		}

	}
}

//Delegate Method
void AEnemySpawner::LaunchRocketSoldierDead(AAIEnemyCharacter* junk) 
{
	if (EventMang) 
	{
		EventMang->BlowUpSubmarineSection1();
	}
}



