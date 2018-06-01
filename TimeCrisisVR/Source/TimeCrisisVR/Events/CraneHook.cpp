// Fill out your copyright notice in the Description page of Project Settings.

#include "CraneHook.h"
#include "Events/CraneHookAnim.h"

void ACraneHook::StartCraneAnimationSequence() 
{
	Cast<UCraneHookAnim>(this->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bShouldMove = true;
}




