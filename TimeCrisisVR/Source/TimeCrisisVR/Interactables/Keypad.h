// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Keypad.generated.h"


UCLASS()
class TIMECRISISVR_API AKeypad : public AActor
{
	GENERATED_BODY()
	
		//Variables
public:

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* KeypadMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UBoxComponent* KeypadMeshChangingTrigger = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UAudioComponent* KeypadAudioComp = nullptr;

	//Key components
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		USceneComponent* KeyParent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UChildActorComponent* Key1 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key2 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key3 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key4 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key5 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key6 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key7 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key8 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* Key9 = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UChildActorComponent* KeyCancel = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Widget")
		class UWidgetComponent* NumberDisplay = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundBase* ClearSFX;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundBase* DialogIntro;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sound")
	TArray<USoundBase*> DialogNumbers;

	//The numbers currently entered into the system
	UPROPERTY(BlueprintReadOnly, Category = "Sound")
	TArray<int32> CodeToEnter;

private:

	TArray<int32> EnteredNumbers;

	int32 CurrentIndexOfEnteredNumbers = 0;

	const int32 CMaxDigitsInCode = 4;

	//Audio Cue Logic
	//If the audio code should be read out loud
	bool bShouldPlayAudioCode = false;

	//The intro will play first and the delegate will cause it to be incremented to 0.
	int32 CurrentNumberBeingRead = -1;

	FTimerHandle AnnouncerVoiceHandle;

	//Methods
public:
	AKeypad();
	virtual void Tick(float DeltaTime) override;

	void PlayDoorAudioCueCPP();

	void StarOutAndDisableButton();

	int32 GetCurrentIndexOfEnteredNum();

	bool CheckIfCodeIsCorrect();


protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);


	void GenerateRandomCode();

	void PlayDoorAudioCueInternalDelegate();

	//Increments the audio clip to read out
	UFUNCTION()
	void AudioCompFinishedPreviousClip();
	
	void ConstantAudioChecker();
	
};
