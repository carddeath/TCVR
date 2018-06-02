// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayersGun.h"
#include "Engine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AmmoClip.h"
#include "Gameplay/Announcer.h"
#include "Managers/NavigationManager.h"
#include "Gameplay/NavigationArrow.h"
#include "AI/AIEnemyCharacter.h"
#include "UI/UIAmmoClip.h"
#include "Components/WidgetComponent.h"
#include "Events/ExplosiveBox.h"


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

	//Get the announcer object in the scene
	for (TActorIterator<AAnnouncer> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Announcer = *ActorItr;
	}

	if (!Announcer) 
	{
		UE_LOG(LogTemp, Error, TEXT("MISSING ANNOUNCER OBJECT IN %s"), *this->GetName());
	}

	//Get the Navigation Manager object in the scene
	for (TActorIterator<ANavigationManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		NavManager = *ActorItr;
	}

	if (!NavManager)
	{
		UE_LOG(LogTemp, Error, TEXT("MISSING NAV MANAGER OBJECT IN %s"), *this->GetName());
	}

	AmmoClipUILeft = Cast<UUIAmmoClip>(LeftGunAmmoWidget2->GetUserWidgetObject());
	AmmoClipUIRight = Cast<UUIAmmoClip>(RightGunAmmoWidget->GetUserWidgetObject());
}

// Called every frame
void APlayersGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayersGun::Fire() 
{
	//Gun Audio
	PlayFireSound();

	//TODO: Fire an actual projectile - Alternative fire method
	//TODO: Add a muzzle flash

	//Remove a bullet and shoot as he have a bullet.
	if (CurrentAmmo > 0) 
	{
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

		TArray<FHitResult> Hits;
		//Invert the Range as it points the other way.
		//if (GetWorld()->LineTraceSingleByObjectType(Hit, ShotStartPos, ShotEndLocation, Params))
		if(GetWorld()->LineTraceMultiByObjectType(Hits, ShotStartPos, ShotEndLocation, Params))
		{
			DrawDebugLine(GetWorld(), ShotStartPos, ShotEndLocation, FColor::Green, false, 2.0f, 0, 2.0f);

			for (auto& actor : Hits)
			{
				if (Cast<ANavigationArrow>(actor.GetActor()))
				{
					if (NavManager)
					{
						//Change false to true if we want "Teleport" behaviour
						NavManager->UpdateCurrentSection(bTeleportOnLocoHit);
						break;
					}
				}
				else if (Cast<AAIEnemyCharacter>(actor.GetActor()))
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
					}
					break;
				}
				else if (Cast<AExplosiveBox>(actor.GetActor()))
				{
					Cast<AExplosiveBox>(actor.GetActor())->TookDamageFromPlayer();
					break;
				}

				if (actor.GetActor()->Tags.Num() > 0)
				{
					PlayImpactSound(actor.GetActor()->Tags[0], actor.GetActor()->GetRootComponent());
					break;
				}

				//Only interested in the first thing we hit
				break;
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), ShotStartPos, ShotEndLocation, FColor::Red, false, 2.0f, 0, 2.0f);
		}
		
		CurrentAmmo--;

		if (AmmoClipUILeft && AmmoClipUIRight) 
		{
			AmmoClipUILeft->UpdateDisplayRemoteCall(false, CurrentAmmo);
			AmmoClipUIRight->UpdateDisplayRemoteCall(false, CurrentAmmo);
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
		if (Announcer) 
		{
			Announcer->PlayReload();
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

