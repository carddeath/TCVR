// Fill out your copyright notice in the Description page of Project Settings.

#include "DataTracker.h"
#include "Managers/NavigationManager.h"
#include "EngineUtils.h"
#include "Player/PlayersGun.h"
#include "Player/VRPawn.h"
#include "Managers/TImeManager.h"
#include "Managers/EnemySpawner.h"


// Sets default values
ADataTracker::ADataTracker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADataTracker::BeginPlay()
{
	Super::BeginPlay();

	for (TActorIterator<ANavigationManager> ita(GetWorld()); ita; ++ita) 
	{
		NavManager = *ita;
	}

	for (TActorIterator<AEnemySpawner> ita(GetWorld()); ita; ++ita)
	{
		EnemySpawner = *ita;
	}

	for (TActorIterator<ATImeManager> ita(GetWorld()); ita; ++ita)
	{
		TimeManager = *ita;
	}

	//Assign this delegate so we pull all the information when it's required
	if (NavManager) 
	{
		NavManager->AllowDataCollectionFromClassesDelegate.AddDynamic(this, &ADataTracker::FindAllData);
	}
}

// Called every frame
void ADataTracker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADataTracker::FindAllData(int junk) 
{
	//Get total times hit
	GameData.TotalTimesHitByEnemies = Cast<AVRPawn>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetTotalTimesHit();

	//Get total shots fired
	if (PlayersGun) 
	{
		GameData.AmountOfBulletsFired = PlayersGun->GetTotalShotsFired();
	}

	//Get total enemies killed
	if (EnemySpawner) 
	{
		GameData.AmountOfBulletsHit = EnemySpawner->GetTotalEnemiesKilled();
	}

	//Get total time
	if (TimeManager) 
	{
		GameData.TotalTimeTaken = TimeManager->GetTotalTimeAtAreaEnd();
	}

	//Get game area
	if (NavManager) 
	{
		GameData.CurrentArea = NavManager->GetCurrentGameArea();
	}

	UE_LOG(LogTemp, Warning, TEXT("TotalTimesHitByEnemy %d"), GameData.TotalTimesHitByEnemies);
	UE_LOG(LogTemp, Warning, TEXT("AMount of shots fired %d"), GameData.AmountOfBulletsFired);
	UE_LOG(LogTemp, Warning, TEXT("Amount of enemies killed %d"), GameData.AmountOfBulletsHit);
	UE_LOG(LogTemp, Warning, TEXT("Time Taken  %f"), GameData.TotalTimeTaken);

	if (DataSendingDelegate.IsBound()) 
	{
		DataSendingDelegate.Broadcast(GameData);
	}
}

