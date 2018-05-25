// Fill out your copyright notice in the Description page of Project Settings.

#include "UIAmmoClip.h"

#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UUIAmmoClip::Construct() 
{

}

void UUIAmmoClip::UpdateDisplay(UTextBlock* AmmoTextUI, UVerticalBox* BulletVBBox, bool bWasReload, int32 AmmoQuantity)
{
	if (AmmoTextUI == nullptr || BulletVBBox == nullptr) return;

	if (bWasReload) 
	{

		for (int32 i = 0; i < BulletVBBox->GetChildrenCount(); i++)
		{
			BulletVBBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Visible);
		}
		AmmoTextUI->SetText(FText::AsNumber(AmmoQuantity));
	}
	else 
	{
		for (int32 i = 0; i < BulletVBBox->GetChildrenCount() - AmmoQuantity; i++)
		{
			BulletVBBox->GetChildAt(i)->SetVisibility(ESlateVisibility::Collapsed);
		}
		AmmoTextUI->SetText(FText::AsNumber(AmmoQuantity));
	}
}




