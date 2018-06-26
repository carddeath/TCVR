// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Managers/GlobalSounds.h"
#include "Components/WidgetComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayersGun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShotTargetInTutorial, int32, junk);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadedInTutorial, int32, junk);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLocomotionArrowInTutorial, int32, junk);


UCLASS()
class TIMECRISISVR_API APlayersGun : public AActor
{
	GENERATED_BODY()


		//Variables
public:

	//Used to check if the gun is in tutorial mode or not
	bool bTutorialHandSwap = false;

	//Tutorial Variables
	FShotTargetInTutorial TutorialTargetShotDelegate;

	bool bTutorialEnabled = false;

	FReloadedInTutorial TutorialReloadedGun;

	FLocomotionArrowInTutorial LocoArrowInTutorial;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* GunStaticMeshComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USceneComponent* FireStartPointComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	class USoundBase* SFXGunShot = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* SFXEmptyClip = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* SFXReloaded = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	class UBoxComponent* ReloadCollider = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UWidgetComponent* LeftGunAmmoWidget2 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UWidgetComponent* RightGunAmmoWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	class UParticleSystemComponent* ParticleComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
		class UAudioComponent* BulletDropAudioComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class UParticleSystem* HitParticleSystem = nullptr;

private:
	UPROPERTY(VisibleAnywhere, Category = "Gun Specifics")
	int32 CurrentAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	int32 FullClipAmmo = 6;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	float GunRange = 2000.0f;

	class AEventManager* EventManager = nullptr;

	class ANavigationManager* NavManager = nullptr;

	class UUIAmmoClip* AmmoClipUILeft = nullptr;

	class UUIAmmoClip* AmmoClipUIRight = nullptr;

	//Tracks the total shots for DataTracker
	int32 TotalShotsFired = 0;

	//The total shots hitting enemies
	int32 TotalShotsHit = 0;

	//This gets turned to false when hitting a nav arrow and enabled when arriving at the next point
	bool bIsAllowedToFire = true;

	//CHANGE: Change to false if required if we want the teleporting basis
	bool bIsNodeBasedMovement = true;

		//Methods
	
public:	
	APlayersGun();
	virtual void Tick(float DeltaTime) override;

	//Called from VRPawn
	void Fire();
	void DisableGripUI(bool bLeftHandHolding);

	//Getters
	int32 GetTotalShotsFired();

	int32 GetTotalShotsHit();

protected:
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintImplementableEvent, Category = "SoundEvents")
		void PlayBulletDropSoundAfterDelay();

private:	

	void PlayFireSound();

	void PlayImpactSound(FName TagOfHitObject, USceneComponent* SoundLocation);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	void AllowGunTobeFired(int junk);

};
