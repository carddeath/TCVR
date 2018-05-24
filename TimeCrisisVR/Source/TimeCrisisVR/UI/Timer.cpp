// Fill out your copyright notice in the Description page of Project Settings.

#include "Timer.h"
#include "Components/TextBlock.h"

void UTimer::AddTimeToTotal(float AmtToAdd) 
{
	//Clamp it between 0 and 60 when adding the time remaining
	FMath::Clamp<float>((TimeRemaining += AmtToAdd), 0.0f, 60.0f);
}