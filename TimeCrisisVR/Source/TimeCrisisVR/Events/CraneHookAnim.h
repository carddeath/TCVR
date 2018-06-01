// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CraneHookAnim.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UCraneHookAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	//If the crane should move or not
	UPROPERTY(BlueprintReadWrite, Category = "Animation State")
		bool bShouldMove = false;
	
	
	
	
};
