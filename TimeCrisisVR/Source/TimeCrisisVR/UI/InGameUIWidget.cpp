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
		AreaStart->SetVisibility(ESlateVisibility::Hidden);
		GetWorld()->GetTimerManager().ClearTimer(FlashAreaStartHandle);
		AmtOfFlashesOnStart = 0;
	}
}

void UInGameUIWidget::FlashUpWait(UTextBlock* WaitTB) 
{
	FTimerDelegate ShowWaitDele = FTimerDelegate::CreateUObject(this, &UInGameUIWidget::FlashUpWaitStartTimer, WaitTB);
	GetWorld()->GetTimerManager().SetTimer(FlashWaitHandle, ShowWaitDele, 0.5f, true);
}

//TODO: We need the function to hide wait when we arrive at the right point and then to display action
void UInGameUIWidget::FlashUpWaitStartTimer(UTextBlock* WaitTB) 
{
	if (WaitTB->GetVisibility() == ESlateVisibility::Hidden)
	{
		WaitTB->SetVisibility(ESlateVisibility::Visible);
		AmtOfFlashesOfWait++;
	}
	else
	{
		WaitTB->SetVisibility(ESlateVisibility::Hidden);
	}

}


