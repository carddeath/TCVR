#pragma once

#include "GameData.generated.h"

USTRUCT(Blueprintable)
struct FGameData
{
	GENERATED_BODY()
	int32 TotalTimesHitByEnemies; //Used for experiment data, not for display
	int32 AmountOfBulletsFired;
	int32 AmountOfBulletsHit; //Also known as the amount of enemies dead (Except for explosions etc)
	int32 CurrentArea; //The current area cleared
	float TotalTimeTaken; //The total time taken on the game so far
	float TopTime; //The top score time
};
