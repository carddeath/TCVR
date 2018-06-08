// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameUIWidget.h"
#include "RunTime/Engine/Public/TimerManager.h"
#include "Components/TextBlock.h"

void UInGameUIWidget::FlashUpAreaStartCallable(UTextBlock* AreaStart) 
{
	FTimerDelegate ShowAreaDele = FTimerDelegate::CreateUObject(this, &UInGameUIWidget::FlashUpAreaStartTimer, AreaStart);
	GetWorld()->GetTimerManager().SetTimer(FlashAreaStartHandle, ShowAreaDele, 0.5f, true);
}

void UInGameUIWidget::FlashUpAreaStartTimer(UTextBlock* AreaStart) 
{

	if (AmtOfFlashesOnStart < 4) 
	{
		if (AreaStart->GetVisibility() == ESlateVisibility::Hidden) 
		{
			AreaStart->SetVisibility(ESlateVisibility::Visible);
			AmtOfFlashesOnStart++;
		}
		else 
		{
			AreaStart->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else 
	{
		AreaStart->SetVisibility(ESlateVisibility::Collapsed);
		GetWorld()->GetTimerManager().ClearTimer(FlashAreaStartHandle);
		AmtOfFlashesOnStart = 0;
		HideWait();
		DisplayAction();
	}
}

void UInGameUIWidget::DisplayWaitCallable(bool bShouldDisplayWait, UTextBlock* WaitTB) 
{
	if (bShouldDisplayWait) 
	{
		WaitTB->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		WaitTB->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInGameUIWidget::DisplayProceedCallable(bool bShouldDisplayProceed, UTextBlock* ProceedTB) 
{
	if (bShouldDisplayProceed) 
	{
		ProceedTB->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		ProceedTB->SetVisibility(ESlateVisibility::Collapsed);
	}
}
