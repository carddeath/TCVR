// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ArmyJeepAnim.generated.h"

/**
 * 
 */
UCLASS()
class TIMECRISISVR_API UArmyJeepAnim : public UAnimInstance
{
	GENERATED_BODY()

public:

	//If the box in the jeep was shot
	UPROPERTY(BlueprintReadWrite, Category = "Anim State")
		bool bDidExplode = false;
	
	
	
	
};
