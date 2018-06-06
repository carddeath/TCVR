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

#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


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
	SetupReloadTime();
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
		case EnemyType::GREEN:
			this->FindComponentByClass<USkeletalMeshComponent>()->SetMaterial(0, EnemyMaterials[3]);
			break;
		case EnemyType::RED:
			this->FindComponentByClass<USkeletalMeshComponent>()->SetMaterial(0, EnemyMaterials[4]);
			break;
		default:
			break;
		}
	}
}

void AAIEnemyCharacter::SetupEnemyPosition() 
{
	//For pop soldiers their EnemyAIController choice is set in the RevealEnemyFromCrouchSpawnPop function

	SetActorLocation(SpawnPoint->GetActorLocation());

	if (MyBehaviour == EAIBehaviour::SPAWN_RUN_SHOOT) 
	{
		//Move to the correct position
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->speed = 15.0f;
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsRunning = true;
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsFiring = false;

		//If they have a GoToPoint we send set the value 
		Cast<AEnemyAIController>(UAIBlueprintHelperLibrary::GetAIController(this))->MoveToPosition(GoToPoint);
	}
	else if (MyBehaviour == EAIBehaviour::SPAWN_RUN_SHOOT_ESCAPE) 
	{
		//Move to the correct position
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->speed = 15.0f;
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsRunning = true;
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsFiring = false;

		//If they have a GoToPoint we send set the value 
		Cast<AEnemyAIController>(UAIBlueprintHelperLibrary::GetAIController(this))->SpawnMoveShootEscape(GoToPoint, EscapePoint);
	}
	else if (MyBehaviour == EAIBehaviour::SPAWN_POP_SHOOT_ADVANCE_SHOOT) 
	{
		//Set the scale to 0 so we can't see the enemy while he is crouching. Note I tried visibility and hiding the actor in game but it messed up the animation
		this->SetActorScale3D(FVector(0.0f));
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsCrouching = true;
		//Make the character be revealed after 0.5 seconds
		GetWorld()->GetTimerManager().SetTimer(CrouchInvisibleDelayHandle, this, &AAIEnemyCharacter::RevealEnemyFromCrouchSpawnAdvance, 1.0f, false);
	}
	else if (MyBehaviour == EAIBehaviour::SPAWN_POP_SHOOT) 
	{
		//Set the scale to 0 so we can't see the enemy while he is crouching. Note I tried visibility and hiding the actor in game but it messed up the animation
		this->SetActorScale3D(FVector(0.0f));
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsCrouching = true;
		//Make the character be revealed after 0.5 seconds
		GetWorld()->GetTimerManager().SetTimer(CrouchInvisibleDelayHandle, this, &AAIEnemyCharacter::RevealEnemyFromCrouchSpawnPop, 1.0f, false);
	}
	else  if (MyBehaviour == EAIBehaviour::SPAWN_SHOOT) 
	{
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsFiring = true;
		Cast<AEnemyAIController>(UAIBlueprintHelperLibrary::GetAIController(this))->SpawnPopShoot();
	}
	else if (MyBehaviour == EAIBehaviour::SPAWN_POP_SHOOT_ESCAPE)
	{
		this->SetActorScale3D(FVector(0.0f));
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsCrouching = true;
		//Make the character be revealed after 0.5 seconds
		GetWorld()->GetTimerManager().SetTimer(CrouchInvisibleDelayHandle, this, &AAIEnemyCharacter::RevealEnemyFromCrouchSpawnPop, 1.0f, false);
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
	//TODO: - Make sure to disable all colliders on the hitboxes of the mesh


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


	if (DeathNoises.Num() >= 3 && !bIsDead)
	{
		bIsDead = true;
		int32 DeathToPlay = FMath::RandRange(0, 3);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), DeathNoises[DeathToPlay], this->GetActorLocation());

		//If we have a random death then pick one.
		if (HitBoxTarget == HitArea::RANDOM) 
		{
			int32 RandDeath = FMath::RandRange(0, 2);
			switch (RandDeath)
			{
			case 0:
				HitBoxTarget = HitArea::HEAD;
				break;
			case 1:
				HitBoxTarget = HitArea::TORSO;
				break;
			case 2:
				HitBoxTarget = HitArea::LEGS;
				break;
			default:
				break;
			}
		}


		//Helps to decide which animation to load in the animator
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->HitAreaSection = HitBoxTarget;
		Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsDead = true;




		TArray<USceneComponent*> Children;
		GetMesh()->GetChildrenComponents(false, Children);

		for (auto& childComp : Children) 
		{
			childComp->DestroyComponent();
		}

		GetCapsuleComponent()->DestroyComponent();

		GetWorldTimerManager().SetTimer(DeathVanishHandle, this, &AAIEnemyCharacter::DeleteEnemy, 3.0f, false);

	}

	//DEBUG: Incase we need to destroy the enemy
	//Destroy();
}

void AAIEnemyCharacter::EraseEnemy() 
{
	if (DeathCallback.IsBound())
	{
		DeathCallback.Broadcast(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Delegate wasn't bound on %s"), *this->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Delegate wasn't bound on AIEnemy Character"));
	}
	Destroy();
}

void AAIEnemyCharacter::DeleteEnemy() 
{
	Destroy();
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
		//UE_LOG(LogTemp, Warning, TEXT("Fire Shot"));
	}
}

//Helper Functions called via timer delegation
void AAIEnemyCharacter::RevealEnemyFromCrouchSpawnAdvance() 
{
	//TODO: Hitboxes should also be disabled until revealed otherwise they can still hit the enemy
	//Set the scale to 1 so we can see the enemy while he is crouching. Note I tried visibility and hiding the actor in game but it messed up the animation
	this->SetActorScale3D(FVector(1.0f));
	//Stand up
	Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsCrouching = false;

	Cast<AEnemyAIController>(UAIBlueprintHelperLibrary::GetAIController(this))->SpawnPopShootAdvanceShoot(AdvancePoint);

}

void AAIEnemyCharacter::RevealEnemyFromCrouchSpawnPop() 
{
	this->SetActorScale3D(FVector(1.0f));
	//Stand up
	Cast<UAIAnimInstance>(FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance())->bIsCrouching = false;

	//Pick the appropriate blackboard values to set
	if (MyBehaviour == EAIBehaviour::SPAWN_POP_SHOOT) 
	{
		Cast<AEnemyAIController>(UAIBlueprintHelperLibrary::GetAIController(this))->SpawnPopShoot();
	}
	else if (MyBehaviour == EAIBehaviour::SPAWN_POP_SHOOT_ESCAPE)
	{
		Cast<AEnemyAIController>(UAIBlueprintHelperLibrary::GetAIController(this))->SpawnPopShootEscape(EscapePoint);
	}

}

//TODO: Remove this once weapon classes are introduced, convert this to setting up the correct weapon class instead
void AAIEnemyCharacter::SetupReloadTime() 
{
	switch (MyWeapon) 
	{
	case EnemyWeapon::PISTOL:
		ReloadDelay = 2.0f;
		break;
	case EnemyWeapon::ROCKET_LAUNCHER:
		ReloadDelay = 1.0f;
		break;
	case EnemyWeapon::SHIELD_PISTOL:
		ReloadDelay = 2.0f;
		break;
	case EnemyWeapon::PROJECTILE:
		ReloadDelay = 1.0f;
		break;
	default:
		break;
	}
}

bool AAIEnemyCharacter::GetDeathState() 
{
	return bIsDead;
}
