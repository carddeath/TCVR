// Fill out your copyright notice in the Description page of Project Settings.

#include "EndOfAreaDisplay.h"
#include "Components/TextBlock.h"

void UEndOfAreaDisplay::PopulateData(class UTextBlock* AreaNum, UTextBlock* MainTimer, UTextBlock* VsTopMain, UTextBlock* AccuracyPer, UTextBlock* ShotRatio) 
{
	//Sets the area number in the title
	AreaNum->SetText(FText::AsNumber(GameData.CurrentArea));

	//Calculate the timer
	TimeStringConstruction(MainTimer, false);

	//Calculate the vs top timer
	TimeStringConstruction(VsTopMain, true);


	//Set the shot ratio
	FString ShotRatioString = "(";
	ShotRatioString.Append(FString::FromInt(GameData.AmountOfBulletsHit));
	ShotRatioString.Append("/");
	ShotRatioString.Append(FString::FromInt(GameData.AmountOfBulletsFired));
	ShotRatioString.Append(")");
	ShotRatio->SetText(FText::FromString(ShotRatioString));

	//Set the accuracy by getting the percentage
	float AccuracyPercent = (float)GameData.AmountOfBulletsHit / GameData.AmountOfBulletsFired;
	AccuracyPercent *= 100;
	//int32 AccuracyPercent  = (GameData.AmountOfBulletsHit / GameData.AmountOfBulletsFired) * 100;
	FString AccString = FString::FromInt(FMath::FloorToInt(AccuracyPercent));
	AccString.Append("%");
	AccuracyPer->SetText(FText::FromString(AccString));

}

void UEndOfAreaDisplay::TimeStringConstruction(UTextBlock* TimeToChange, bool bIsVsTime) 
{
	int32 Mins = 0;
	int32 Secs = 0;
	int32 MilliSecs = 0;
	float TotalTime = GameData.TotalTimeTaken;
	FString MainTimerString;

	//If it's the vs time we need to generate a symbol of positive or negative
	if (bIsVsTime) 
	{
		if (TotalTime < GameData.TopTime) 
		{
			MainTimerString.Append("-");
			TotalTime = GameData.TopTime -= TotalTime ; //Find the remaining time left to know if it's positive or negative
		}
		else 
		{
			MainTimerString.Append("+");
			TotalTime -= GameData.TopTime; //Find the remaining time left to know if it's positive or negative
		}
	}

	//If we have more than 60 seconds
	if (TotalTime >= 60.0f)
	{
		Mins = ((int32)TotalTime / 60) % 60;
		Secs = (int32)TotalTime % 60;
		MilliSecs = (Secs) % 60;

		//Add a 0 at the start if were under 10 mins
		if (Mins < 10) 
		{

			MainTimerString.Append("0");
		}

		MainTimerString.Append(FString::FromInt(Mins));
	}
	else
	{
		Secs = (int32)TotalTime % 60;
		MilliSecs = (Secs) % 60;
		MainTimerString.Append("00");
	}

	MainTimerString.Append("'");

	if (Secs < 10) 
	{
		MainTimerString.Append("0");
	}
	MainTimerString.Append(FString::FromInt(Secs));
	MainTimerString.Append("''");

	if (MilliSecs < 10) 
	{
		MainTimerString.Append("0");
	}
	MainTimerString.Append(FString::FromInt(MilliSecs));

	TimeToChange->SetText(FText::FromString(MainTimerString));
}


