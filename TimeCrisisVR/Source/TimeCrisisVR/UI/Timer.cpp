// Fill out your copyright notice in the Description page of Project Settings.

#include "Timer.h"
#include "Components/TextBlock.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

void UTimer::AddTimeToTotal(float AmtToAdd) 
{
	//Clamp it between 0 and 60 when adding the time remaining
	TimeRemaining += AmtToAdd;
	TimeRemaining = FMath::Clamp<float>(TimeRemaining, 0.0f, 60.0f);
	
	if (TimeAddedSFX) 
	{
		UGameplayStatics::PlaySound2D(GetWorld(), TimeAddedSFX);
		FlashTimeExtended();
	}

}

void UTimer::TogglePauseOnTimer(bool bShouldPause) 
{
	bIsTimerPaused = bShouldPause;
}

void UTimer::WasEndOfGame() 
{
	if (EndOfGameTriggerDelegate.IsBound()) 
	{
		EndOfGameTriggerDelegate.Broadcast(0);
	}
}