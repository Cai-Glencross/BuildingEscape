// Copyright Cai Glencross 2022.


#include "OpenDoor.h"
#include "Components/AudioComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	FRotator CurrentRotator = GetOwner()->GetActorRotation();
	Audio = GetOwner()->FindComponentByClass<UAudioComponent>();

	InitialYaw = CurrentRotator.Yaw;

	TargetYaw += InitialYaw;

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("NO PRESSURE PLATE SET ON %s"), *GetOwner()->GetName());
	}else {
		UE_LOG(LogTemp, Warning, TEXT("Pressure Plate Position is: %s"), *PressurePlate->GetActorLocation().ToString());
	}

	OpenSoundHasPlayed = false;
	ClosedSoundHasPlayed = true;

	DoorLastOpened = GetWorld()->GetTimeSeconds();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(TotalMassOfActors() > MassThreshold){
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} else {
		if (DoorLastOpened + DoorCloseDelay < GetWorld()->GetTimeSeconds()) {
			CloseDoor(DeltaTime);
		}	
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw, DeltaTime, 1);
	GetOwner()->SetActorRotation(CurrentRotation);
	if(Audio && !OpenSoundHasPlayed) {
		UE_LOG(LogTemp, Warning, TEXT("playing sound in open!!!"));
		Audio->Play();
		OpenSoundHasPlayed = true;
		ClosedSoundHasPlayed = false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	FRotator CurrentRotation = GetOwner()->GetActorRotation();
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, InitialYaw, DeltaTime, 1);
	GetOwner()->SetActorRotation(CurrentRotation);
	if(Audio && !ClosedSoundHasPlayed) {
		UE_LOG(LogTemp, Warning, TEXT("playing sound in close!!!"));
		Audio->Play();
		ClosedSoundHasPlayed = true;
		OpenSoundHasPlayed = false;
	}
}

float UOpenDoor::TotalMassOfActors() const {
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	if (PressurePlate) {
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	}
	
	for (AActor* Actor : OverlappingActors) {
		if (Actor && Actor->FindComponentByClass<UPrimitiveComponent>()) {
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
		
	}

	return TotalMass;
}

