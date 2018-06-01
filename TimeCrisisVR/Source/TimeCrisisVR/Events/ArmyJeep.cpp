// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmyJeep.h"
#include "Events/ArmyJeepAnim.h"

void AArmyJeep::StartExplodedAnimation() 
{
	Cast<UArmyJeepAnim>(this->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bDidExplode = true;
}



