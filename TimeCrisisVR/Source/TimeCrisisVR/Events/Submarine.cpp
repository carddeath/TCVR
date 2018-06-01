// Fill out your copyright notice in the Description page of Project Settings.

#include "Submarine.h"
#include "Events/SubmarineAnim.h"

void ASubmarine::StartSubmarineSinkSequence() 
{
	Cast<USubmarineAnim>(this->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsSinking = true;
}




