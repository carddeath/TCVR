// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SubmarineAnim.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API USubmarineAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Anim Settings")
		bool bIsSinking = false;
	
	
	
};
