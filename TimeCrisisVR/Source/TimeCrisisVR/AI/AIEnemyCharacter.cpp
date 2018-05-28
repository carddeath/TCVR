// Fill out your copyright notice in the Description page of Project Settings.

#include "AIEnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/TargetPoint.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "AI/EnemyAIController.h"
#include "Classes/AIController.h"
#include "Engine.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Public/TimerManager.h"


// Sets default values
AAIEnemyCharacter::AAIEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAIEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAIEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: NEEDS TESTING - However should grant a threshold when movement takes place to see if the unit is close enough to the go to location.
	if (GoToPoint != nullptr) 
	{
		if (GetActorLocation().X > GoToPoint->GetActorLocation().X + 2.0f || GetActorLocation().X < GoToPoint->GetActorLocation().X - 2.0f
			|| GetActorLocation().Z > GoToPoint->GetActorLocation().Z + 2.0f || GetActorLocation().Z < GoToPoint->GetActorLocation().Z - 2.0f && !bIsAtGoToLocation)
		{
			//bIsAtGoToLocation = true;
		}
	}


	//Only rotate to face the player if the unit is in the correct position
	if (bIsAtGoToLocation) 
	{
		//Don't want this enemy to lean so 0 out the z direction
		FVector dir = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
		dir.Z = 0.0f;

		//Create a matrix on the X axis to pivot the unit around from the players location to this units location and set it
		FRotator lookAtRot = FRotationMatrix::MakeFromX(dir - GetActorLocation()).Rotator();
		SetActorRotation(lookAtRot);
	}

}

void AAIEnemyCharacter::SetupEnemy(EnemyType typeOfEnemy, EnemyWeapon EnemyWep, EAIBehaviour Behaviour, ATargetPoint* SpwnPoint, ATargetPoint* GoToPnt, ATargetPoint* EscPoint, ATargetPoint* AdvancePnt)
{
	MyType = typeOfEnemy;
	MyWeapon = EnemyWep;
	MyBehaviour = Behaviour;
	SpawnPoint = SpwnPoint;
	GoToPoint = GoToPnt;
	EscapePoint = EscPoint;
	AdvancePoint = AdvancePnt;

	SetupEnemyPosition();
	TextureEnemy();
	SetupEnemyWeaponAnimationState();
}

void AAIEnemyCharacter::TextureEnemy() 
{
	if (EnemyMaterials.Num() > 0) 
	{
		switch (MyType)
		{
		case EnemyType::BLUE:
			this->FindComponentByClass<USkeletalMeshComponent>()->SetMaterial(0, EnemyMaterials[0]);
			break;
		case EnemyType::BROWN:
			this->FindComponentByClass<USkeletalMeshComponent>()->SetMaterial(0, EnemyMaterials[1]);
			break;
		case EnemyType::ORANGE:
			this->FindComponentByClass<USkeletalMeshComponent>()->SetMaterial(0, EnemyMaterials[2]);
			break;
		default:
			break;
		}
	}
}

void AAIEnemyCharacter::SetupEnemyPosition() 
{
	SetActorLocation(SpawnPoint->GetActorLocation());

	//If the enemy shouldn't move
	if (MyBehaviour == EAIBehaviour::SPAWN_RUN_SHOOT) 
	{
		//Move to the correct position
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->speed = 15.0f;

		//If they have a GoToPoint we send set the value 
		Cast<AEnemyAIController>(UAIBlueprintHelperLibrary::GetAIController(this))->MoveToPosition(GoToPoint);
	}
	else if (MyBehaviour == EAIBehaviour::SPAWN_RUN_SHOOT_ESCAPE) 
	{
		//Move to the correct position
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->speed = 15.0f;

		//If they have a GoToPoint we send set the value 
		Cast<AEnemyAIController>(UAIBlueprintHelperLibrary::GetAIController(this))->SpawnMoveShootEscape(GoToPoint, EscapePoint);
	}
	else if (MyBehaviour == EAIBehaviour::SPAWN_POP_SHOOT_ADVANCE_SHOOT) 
	{
		//Start the unit as crouching, we will disable this after a delay in the BT (Behaviour Tree)
		//Set the scale to 0 so we can't see the enemy while he is crouching. Note I tried visibility and hiding the actor in game but it messed up the animation
		this->SetActorScale3D(FVector(0.0f));
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsCrouching = true;
		//Make the character be revealed after 0.5 seconds
		GetWorld()->GetTimerManager().SetTimer(CrouchInvisibleDelayHandle, this, &AAIEnemyCharacter::RevealEnemyFromCrouchSpawn, 1.0f, false);
	}
	else 
	{
		//TODO: Need to also set this when the player arrives at the correct position
		bIsAtGoToLocation = true;
	}
}

//Sends the correct weapon type to the character so they know which shooting animation to use
void AAIEnemyCharacter::SetupEnemyWeaponAnimationState() 
{
	Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->WeaponInUse = MyWeapon;
}

// Called to bind functionality to input
void AAIEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAIEnemyCharacter::KillEnemy(HitArea HitBoxTarget)
{
	//TODO - Randomise the death noise
	//TODO: - Make sure to disable all colliders on the hitboxes of the mess

	//Make sure we've actually bound the delegate
	if (DeathCallback.IsBound()) 
	{
		DeathCallback.Broadcast(this);
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("Delegate wasn't bound on %s"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Delegate wasn't bound on AIEnemy Character"));
	}


	if (DeathNoises.Num() > 0 && !bIsDead)
	{
		bIsDead = true;
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DeathNoises[0], this->GetActorLocation());

		//Helps to decide which animation to load in the animator
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->HitAreaSection = HitBoxTarget;
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsDead = true;

		//Debug only
		//switch (HitBoxTarget) 
		//{
		//case HitArea::LEGS:
		//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, TEXT("LEG shot hit!"));
		//	break;
		//case HitArea::TORSO:
		//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("TORSO shot hit!"));
		//	break;
		//case HitArea::HEAD:
		//	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, TEXT("HEAD shot hit!"));
		//	break;
		//default:
		//	break;
		//}

	}

	//DEBUG: Incase we need to destroy the enemy
	Destroy();
}

void AAIEnemyCharacter::EraseEnemy() 
{
	if (DeathCallback.IsBound())
	{
		DeathCallback.Broadcast(this);
		Destroy();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Delegate wasn't bound on %s"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Delegate wasn't bound on AIEnemy Character"));
	}
}

void AAIEnemyCharacter::FireUponPlayer()
{
	//TODO: Spawn bullets that go towards the player and destroy them
	//TODO: Register damage on the player via the bullet class
	if (bIsReloading) 
	{
		GetWorld()->GetTimerManager().ClearTimer(ShotDelayHandle);
	}
	else 
	{
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsFiring = true;
	//	GetWorld()->GetTimerManager().SetTimer(ShotDelayHandle, this, &AAIEnemyCharacter::SpawnProjectile, 0.5f, true);
		UE_LOG(LogTemp, Warning, TEXT("Fire Shot"));
	}
}

//Helper Functions called via timer delegation
void AAIEnemyCharacter::RevealEnemyFromCrouchSpawn() 
{
	//TODO: Hitboxes should also be disabled until revealed otherwise they can still hit the enemy
	//Set the scale to 1 so we can see the enemy while he is crouching. Note I tried visibility and hiding the actor in game but it messed up the animation
	this->SetActorScale3D(FVector(1.0f));

}
