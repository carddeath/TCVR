// Fill out your copyright notice in the Description page of Project Settings.

#include "Keypad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Interactables/KeypadButton.h"
#include "Engine.h"
#include "Components/WidgetComponent.h"
#include "Interactables/KeypadWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

// Sets default values
AKeypad::AKeypad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeypadMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Keypad Mesh"));
	KeypadMeshChangingTrigger = CreateDefaultSubobject<UBoxComponent>(FName("Hand Changing Box Collision"));

	RootComponent = KeypadMesh;
	KeypadMeshChangingTrigger->SetupAttachment(RootComponent);

	KeypadAudioComp = CreateDefaultSubobject<UAudioComponent>(FName("Audio Component"));
	KeypadAudioComp->SetupAttachment(RootComponent);

	KeyParent = CreateDefaultSubobject<USceneComponent>(FName("KeypadTransform"));
	Key1 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 1"));
	Key2 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 2"));
	Key3 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 3"));
	Key4 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 4"));
	Key5 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 5"));
	Key6 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 6"));
	Key7 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 7"));
	Key8 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 8"));
	Key9 = CreateDefaultSubobject<UChildActorComponent>(FName("Key 9"));
	KeyCancel = CreateDefaultSubobject<UChildActorComponent>(FName("Key Cancel"));

	KeyParent->SetupAttachment(RootComponent);

	Key1->SetupAttachment(KeyParent);
	Key2->SetupAttachment(KeyParent);
	Key3->SetupAttachment(KeyParent);
	Key4->SetupAttachment(KeyParent);
	Key5->SetupAttachment(KeyParent);
	Key6->SetupAttachment(KeyParent);
	Key7->SetupAttachment(KeyParent);
	Key8->SetupAttachment(KeyParent);
	Key9->SetupAttachment(KeyParent);
	KeyCancel->SetupAttachment(KeyParent);

	NumberDisplay = CreateDefaultSubobject<UWidgetComponent>(FName("Number Display Component"));
	NumberDisplay->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AKeypad::BeginPlay()
{
	Super::BeginPlay();

	CodeToEnter.SetNum(4);
	EnteredNumbers.SetNum(4);

	//Need to force the rotation and location of the keys
	//KeyParent->SetRelativeLocation(FVector(9.0f, 4.419f, 23.0f));
	KeyParent->SetRelativeRotation(FRotator(0.0f));

	Cast<AKeypadButton>(Key1->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(Key2->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(Key3->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(Key4->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(Key5->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(Key6->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(Key7->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(Key8->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(Key9->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);
	Cast<AKeypadButton>(KeyCancel->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.AddDynamic(this, &AKeypad::OnComponentBeginOverlap);

	GenerateRandomCode();
	KeypadAudioComp->OnAudioFinished.AddDynamic(this, &AKeypad::AudioCompFinishedPreviousClip);
}

// Called every frame
void AKeypad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Checks if the code should be read out
	ConstantAudioChecker();

}

void AKeypad::GenerateRandomCode() 
{
	for (int i = 0; i < CMaxDigitsInCode; i++)
	{
		CodeToEnter[i] = FMath::RandRange(1, 9);
		UE_LOG(LogTemp, Warning, TEXT("%d"), CodeToEnter[i]);
	}
}

void AKeypad::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//Might not work as a hand holding something may be able to collide
	if (OtherComp->GetName() == "SMLeft" || OtherComp->GetName() == "SMRight")
	{

		if (Cast<AKeypadButton>(OverlappedComponent->GetOwner()))
		{

			int32 EnteredNumber = Cast<AKeypadButton>(OverlappedComponent->GetOwner())->GetNumberKey();

			Cast<UKeypadWidget>(NumberDisplay->GetUserWidgetObject())->InsertNewDigitIntoSystem(EnteredNumber);

			//If we can fit the number store the data
			if (CurrentIndexOfEnteredNumbers < CMaxDigitsInCode)
			{
				//Store the data and increment the counter
				EnteredNumbers[CurrentIndexOfEnteredNumbers] = EnteredNumber;
				CurrentIndexOfEnteredNumbers++;

				//We have now gone over the code limit
				if (CurrentIndexOfEnteredNumbers >= CMaxDigitsInCode) 
				{
					CheckIfCodeIsCorrect();
				}
			}

			for (auto& elm : EnteredNumbers) 
			{
				UE_LOG(LogTemp, Warning, TEXT("%d"), elm);
			}

			UE_LOG(LogTemp, Warning, TEXT("-----------------------"));
			
			//Clear was pressed
			if (EnteredNumber == 0 && ClearSFX) 
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ClearSFX, this->GetActorLocation());

				//Reset the data and counter
				for (auto& num : EnteredNumbers) 
				{
					num = 0;
				}
				CurrentIndexOfEnteredNumbers = 0;
			}
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Fail"));
		}


	}
}



void AKeypad::PlayDoorAudioCueCPP() 
{
	//Sets the timer up so we can cancel it in here once the code is correct
	GetWorldTimerManager().SetTimer(AnnouncerVoiceHandle, this, &AKeypad::PlayDoorAudioCueInternalDelegate, 8.0f, true, 1.0f);
}

void AKeypad::PlayDoorAudioCueInternalDelegate()
{
	if (DialogIntro)
	{
		KeypadAudioComp->SetSound(DialogIntro);
		KeypadAudioComp->Play();
		bShouldPlayAudioCode = true;
	}

}

void AKeypad::AudioCompFinishedPreviousClip() 
{
	CurrentNumberBeingRead++;
}

void AKeypad::ConstantAudioChecker() 
{
	//If we should play but the current clip has ended lets play another
	if (bShouldPlayAudioCode && !KeypadAudioComp->IsPlaying()) 
	{
		//Only read the next digit if we're the correct size
		if (CurrentNumberBeingRead < CMaxDigitsInCode) 
		{
			int32 DigitToRead = CodeToEnter[CurrentNumberBeingRead];
			DigitToRead -= 1;

			//We subtract 1 due to the array index being on 0
			KeypadAudioComp->SetSound(DialogNumbers[DigitToRead]);
			KeypadAudioComp->Play();
		}
		else 
		{
			//Reset once we go over the max amount of digits
			bShouldPlayAudioCode = false;
			CurrentNumberBeingRead = -1;
		}
	}
}

bool AKeypad::CheckIfCodeIsCorrect() 
{
	for (int i = 0; i < CMaxDigitsInCode; i++) 
	{
		if (CodeToEnter[i] != EnteredNumbers[i]) 
		{
			return false; 
		}
	}

	GetWorldTimerManager().ClearTimer(AnnouncerVoiceHandle);
	//If we got past the checks then both arrays match
	UE_LOG(LogTemp, Warning, TEXT("Correct Code entered"));
	return true;
}

void AKeypad::StarOutAndDisableButton() 
{
	//Sets the display to stars
	Cast<UKeypadWidget>(NumberDisplay->GetUserWidgetObject())->SetAllDisplayToStarsEvent();

	//Remove all the delegates, the gate wont close
	Cast<AKeypadButton>(Key1->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(Key2->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(Key3->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(Key4->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(Key5->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(Key6->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(Key7->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(Key8->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(Key9->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
	Cast<AKeypadButton>(KeyCancel->GetChildActor())->FindComponentByClass<UBoxComponent>()->OnComponentBeginOverlap.RemoveAll(this);
}

//Getter

int32 AKeypad::GetCurrentIndexOfEnteredNum() 
{
	return CurrentIndexOfEnteredNumbers;
}

