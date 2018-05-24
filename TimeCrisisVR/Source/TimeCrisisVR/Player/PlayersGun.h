// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Managers/GlobalSounds.h"
#include "Components/WidgetComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayersGun.generated.h"



//Things for this class
//Sound effects and audio components


UCLASS()
class TIMECRISISVR_API APlayersGun : public AActor
{
	GENERATED_BODY()


		//Variables
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* GunStaticMeshComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class USceneComponent* FireStartPointComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	class USoundBase* SFXGunShot = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	USoundBase* SFXEmptyClip = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
		USoundBase* SFXReloaded = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	class UBoxComponent* ReloadCollider = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UWidgetComponent* LeftGunAmmoWidget2 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UWidgetComponent* RightGunAmmoWidget = nullptr;



private:

	UPROPERTY(VisibleAnywhere, Category = "Gun Specifics")
	int32 CurrentAmmo;
	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	int32 FullClipAmmo = 6;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	bool bTeleportOnLocoHit = false;

	UPROPERTY(EditDefaultsOnly, Category = "Gun Specifics")
	float GunRange = 2000.0f;

	class AAnnouncer* Announcer = nullptr;

	class ANavigationManager* NavManager = nullptr;

	class UUIAmmoClip* AmmoClipUILeft = nullptr;

	class UUIAmmoClip* AmmoClipUIRight = nullptr;

		//Methods
	
public:	
	APlayersGun();
	virtual void Tick(float DeltaTime) override;

	//Called from VRPawn
	void Fire();
	void DisableGripUI(bool bLeftHandHolding);

protected:
	virtual void BeginPlay() override;

private:	

	void PlayFireSound();

	void PlayImpactSound(FName TagOfHitObject, USceneComponent* SoundLocation);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

};
