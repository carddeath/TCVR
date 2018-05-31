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


// Sets default values
AKeypad::AKeypad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KeypadMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Keypad Mesh"));
	KeypadMeshChangingTrigger = CreateDefaultSubobject<UBoxComponent>(FName("Hand Changing Box Collision"));

	RootComponent = KeypadMesh;
	KeypadMeshChangingTrigger->SetupAttachment(RootComponent);

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
}

// Called every frame
void AKeypad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
			
			//Clear was pressed
			if (EnteredNumber == 0 && ClearSFX) 
			{
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ClearSFX, this->GetActorLocation());
			}
		}
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Fail"));
		}


	}
}

