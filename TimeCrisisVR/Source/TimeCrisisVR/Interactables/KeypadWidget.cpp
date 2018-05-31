// Fill out your copyright notice in the Description page of Project Settings.

#include "KeypadWidget.h"
#include "Components/TextBlock.h"

void UKeypadWidget::ResetAllText(TArray<UTextBlock*> Digits)
{
	for (auto& TB : Digits) 
	{
		TB->SetText(FText::FromString("_"));
	}

	//Also reset the digit in sequence
	CurrentDigitInSequence = 0;
}




