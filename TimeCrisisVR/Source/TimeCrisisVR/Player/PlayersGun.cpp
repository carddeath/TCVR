// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayersGun.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AmmoClip.h"
#include "Managers/EventManager.h"
#include "Managers/NavigationManager.h"
#include "Gameplay/NavigationArrow.h"
#include "AI/AIEnemyCharacter.h"
#include "UI/UIAmmoClip.h"
#include "Components/WidgetComponent.h"
#include "Events/ExplosiveBox.h"
#include "GameFramework/DamageType.h"
#include "Tutorial/TutorialShootingTarget.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystem.h"


// Sets default values
APlayersGun::APlayersGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GunStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("Gun StaticMesh Comp"));
	GunStaticMeshComp->SetSimulatePhysics(true);
	SetRootComponent(GunStaticMeshComp);

	FireStartPointComp = CreateDefaultSubobject<USceneComponent>(FName("FireStartPoint"));
	FireStartPointComp->SetupAttachment(GunStaticMeshComp);

	ReloadCollider = CreateDefaultSubobject<UBoxComponent>(FName("Reload Collision"));
	ReloadCollider->SetupAttachment(GunStaticMeshComp);

	LeftGunAmmoWidget2 = CreateDefaultSubobject<UWidgetComponent>(FName("Left Ammo Widget 2"));
	LeftGunAmmoWidget2->SetupAttachment(GunStaticMeshComp);

	RightGunAmmoWidget = CreateDefaultSubobject<UWidgetComponent>(FName("Right Ammo Widget"));
	RightGunAmmoWidget->SetupAttachment(GunStaticMeshComp);

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(FName("Particle System"));
	ParticleComponent->SetAutoAttachmentParameters(GunStaticMeshComp, FName("FlashSocket"), EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative);

	BulletDropAudioComp = CreateDefaultSubobject<UAudioComponent>(FName("Bullet Drop Audio Comp"));
	BulletDropAudioComp->SetupAttachment(GunStaticMeshComp);
}

// Called when the game starts or when spawned
void APlayersGun::BeginPlay()
{
	Super::BeginPlay();
	//Sets the physics, not currently working...
	GunStaticMeshComp->SetSimulatePhysics(true);

	//Set the ammo
	CurrentAmmo = FullClipAmmo;	

	//Assign the reload delegate to know what to do when a clip collides with it.
	ReloadCollider->OnComponentBeginOverlap.AddDynamic(this, &APlayersGun::OnComponentBeginOverlap);

	//Get the Event manager object in the scene
	for (TActorIterator<AEventManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		EventManager = *ActorItr;

		if (!EventManager)
		{
			UE_LOG(LogTemp, Error, TEXT("MISSING Event Manager OBJECT IN %s"), *this->GetName());
		}
	}

	//Get the Navigation Manager object in the scene
	for (TActorIterator<ANavigationManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		NavManager = *ActorItr;

		if (!NavManager)
		{
			UE_LOG(LogTemp, Error, TEXT("MISSING NAV MANAGER OBJECT IN %s"), *this->GetName());
		}
	}

	AmmoClipUILeft = Cast<UUIAmmoClip>(LeftGunAmmoWidget2->GetUserWidgetObject());
	AmmoClipUIRight = Cast<UUIAmmoClip>(RightGunAmmoWidget->GetUserWidgetObject());

	//A delegate to allow the gun to be fired once we reach our destination
	if (NavManager) 
	{
		NavManager->TurnGunToEnabled.AddDynamic(this, &APlayersGun::AllowGunTobeFired);
	}
}

// Called every frame
void APlayersGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayersGun::Fire() 
{
	//We are moving currently and are not allowed to fire. Or we are in the hand swapping phase of the tutorial
	if (!bIsAllowedToFire || bTutorialHandSwap)
	{
		return;
	}

	//Gun Audio
	PlayFireSound();

	//Remove a bullet and shoot as he have a bullet.
	if (CurrentAmmo > 0) 
	{
		//Play the bullet drop sound
		PlayBulletDropSoundAfterDelay();
		//Show muzzle flash
		ParticleComponent->Activate(true);

		//Increments all the shots that are fired
		TotalShotsFired++;

		//Fire the raytace
		FVector ShotStartPos = FireStartPointComp->GetComponentLocation();
		FVector ShotEndLocation = FireStartPointComp->GetComponentLocation() + GunStaticMeshComp->GetForwardVector() * -GunRange;
		FHitResult Hit;
		FCollisionObjectQueryParams Params;
		//TODO: Need to have a new channel for all things to be hit that reacts to channels.
		Params.AddObjectTypesToQuery(ECC_WorldStatic);
		Params.AddObjectTypesToQuery(ECC_PhysicsBody);
		Params.AddObjectTypesToQuery(ECC_WorldDynamic);
		Params.AddObjectTypesToQuery(ECC_Destructible);
		Params.AddObjectTypesToQuery(ECC_GameTraceChannel3);

		TArray<FHitResult> Hits;
		//Invert the Range as it points the other way.
		//if (GetWorld()->LineTraceSingleByObjectType(Hit, ShotStartPos, ShotEndLocation, Params))
		if(GetWorld()->LineTraceMultiByObjectType(Hits, ShotStartPos, ShotEndLocation, Params))
		{
			DrawDebugLine(GetWorld(), ShotStartPos, ShotEndLocation, FColor::Green, false, 0.25f, 0, 0.5f);

			for (auto& actor : Hits)
			{
				if (Cast<ANavigationArrow>(actor.GetActor()))
				{
					if (NavManager)
					{
						//We cannot fire until we reach the next point
						bIsAllowedToFire = false;
						//Change false to true if we want "Teleport" behaviour
						NavManager->UpdateCurrentSection();
						TotalShotsHit++;

						//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleSystem, actor.GetActor()->GetActorTransform());

						break;
					}
					//Tutorial logic, we have no navigation manager
					else 
					{
						if (LocoArrowInTutorial.IsBound()) 
						{
							LocoArrowInTutorial.Broadcast(0);
						}
						//Load the main level from the tutorial level when shot
						//EventManager->LoadMainLevelFromTutorial();
					}
				}
				else if (Cast<UShapeComponent>(actor.GetComponent()))
				{
					UShapeComponent* HitBox = Cast<UShapeComponent>(actor.GetComponent());

					//If we have a hitbox
					if (HitBox)
					{
						//If we hit them in the legs
						if (HitBox->ComponentHasTag(FName("Legs")))
						{
							Cast<AAIEnemyCharacter>(actor.GetActor())->KillEnemy(HitArea::LEGS);
						}
						else if (HitBox->ComponentHasTag(FName("Torso")))
						{
							Cast<AAIEnemyCharacter>(actor.GetActor())->KillEnemy(HitArea::TORSO);
						}
						else if (HitBox->ComponentHasTag(FName("Head")))
						{
							Cast<AAIEnemyCharacter>(actor.GetActor())->KillEnemy(HitArea::HEAD);
						}
						TotalShotsHit++;
					}
					break;
				}
				//TODO multiple checks here for areas that contain explosive boxes
				else if (Cast<AExplosiveBox>(actor.GetActor()) && NavManager->GetCurrentGameSection() == 5)
				{
					TotalShotsHit++;
					//If the box blew up
					if (Cast<AExplosiveBox>(actor.GetActor())->TookDamageFromPlayer()) 
					{
						//Kills all enemies in the scene
						for (TActorIterator<AAIEnemyCharacter> EnemyIta(GetWorld()); EnemyIta; ++EnemyIta) 
						{
							//If the character isn't already dead then kill it
							AAIEnemyCharacter* TempChar = *EnemyIta;
							if (!TempChar->GetDeathState()) 
							{
								TempChar->bWasKilledByExplosion = true;
								TempChar->KillEnemy(HitArea::RANDOM);
							}

						}
					}
					break;
				}
				//For Tutorial
				else if (Cast<ATutorialShootingTarget>(actor.GetActor())) 
				{
					if (TutorialTargetShotDelegate.IsBound()) 
					{
						TutorialTargetShotDelegate.Broadcast(0);
						//Clear the delegate
						TutorialTargetShotDelegate.Clear();
					}
					break;
				}

				if (actor.GetActor()->Tags.Num() > 0)
				{
					PlayImpactSound(actor.GetActor()->Tags[0], actor.GetActor()->GetRootComponent());

					//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleSystem, actor.GetActor()->GetTransform());
					break;
				}

				//Only interested in the first thing we hit
				break;
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), ShotStartPos, ShotEndLocation, FColor::Red, false, 0.25f, 0, 0.5f);
		}
		
		CurrentAmmo--;

		if (AmmoClipUILeft && AmmoClipUIRight) 
		{
			AmmoClipUILeft->UpdateDisplayRemoteCall(false, CurrentAmmo);
			AmmoClipUIRight->UpdateDisplayRemoteCall(false, CurrentAmmo);
		}
	}
	//If we are out of ammo in the tutorial and it's bound let's move on the tutorial
	else if(bTutorialEnabled)
	{
		if (TutorialTargetShotDelegate.IsBound()) 
		{
			TutorialTargetShotDelegate.Broadcast(0);
			//Clear the delegate
			TutorialTargetShotDelegate.Clear();
		}
	}
}

void APlayersGun::PlayFireSound() 
{
	if (!SFXEmptyClip || !SFXGunShot) return;

	if (CurrentAmmo <= 0) 
	{
		//Play empty clip and the announcer audio
		UGameplayStatics::SpawnSoundAttached(SFXEmptyClip, this->GetRootComponent());
		if (EventManager) 
		{
			EventManager->AnnouncerReloadCallThroughAndUIBroadcast();
		}
	}
	else 
	{
		UGameplayStatics::SpawnSoundAttached(SFXGunShot, this->GetRootComponent());
	}
}

void APlayersGun::PlayImpactSound(FName TagOfHitObject, USceneComponent* SoundLocation)
{
	TSharedRef<GlobalSounds> GS(new GlobalSounds());
	USoundBase* SoundToPlay = nullptr;



	if (TagOfHitObject.Compare("Metal") == 0)
	{
		SoundToPlay = GS.Get().MetalBulletImpact;
	}
	//TODO: DO this for all "Impact sounds"


	if (SoundToPlay != nullptr)
	{
		UGameplayStatics::SpawnSoundAttached(SoundToPlay, SoundLocation);
	}
}

//Collision

void APlayersGun::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) 
{
	//Try to see if it's an ammo clip thats collided
	AAmmoClip* Ammo = Cast<AAmmoClip>(OtherActor);
	if (Ammo != nullptr) 
	{
		CurrentAmmo = Ammo->GetAmmo();
		OtherActor->Destroy();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("RELOAD!"), true, FVector2D(3.0f, 3.0f));
		UGameplayStatics::SpawnSoundAttached(SFXReloaded, this->GetRootComponent());

		//Update the widget with the ammo in it
		if (AmmoClipUILeft && AmmoClipUIRight)
		{
			AmmoClipUILeft->UpdateDisplayRemoteCall(true, CurrentAmmo);
			AmmoClipUIRight->UpdateDisplayRemoteCall(true, CurrentAmmo);
		}

		//We want to move the tutorial step further with a reload action taking place.
		if (bTutorialEnabled) 
		{
			if (TutorialReloadedGun.IsBound()) 
			{
				TutorialReloadedGun.Broadcast(0);
				TutorialReloadedGun.Clear();
			}
		}
	}
}

//Visual editing via vr pawn
void APlayersGun::DisableGripUI(bool bLeftHandHolding) 
{
	//Hide right UI for ammo count
	if (bLeftHandHolding) 
	{
		AmmoClipUILeft->SetVisibility(ESlateVisibility::Hidden);
		AmmoClipUIRight->SetVisibility(ESlateVisibility::Visible);
	}
	else 
	{
		AmmoClipUILeft->SetVisibility(ESlateVisibility::Visible);
		AmmoClipUIRight->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APlayersGun::AllowGunTobeFired(int junk) 
{
	bIsAllowedToFire = true;
}

//Getters

int32 APlayersGun::GetTotalShotsFired()
{
	return TotalShotsFired;
}

int32 APlayersGun::GetTotalShotsHit() 
{
	return TotalShotsHit;
}

