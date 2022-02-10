// Copyright Cai Glencross 2022.


#include "SlideDoor.h"

USlideDoor::USlideDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void USlideDoor::BeginPlay()
{
	Super::BeginPlay();

	if(!PressurePlate) {
		UE_LOG(LogTemp, Warning, TEXT("No pressure plate attached to object %s"), *GetOwner()->GetName());
	}

	Audio = GetOwner()->FindComponentByClass<UAudioComponent>();

	OpenSoundHasPlayed = false;
	ClosedSoundHasPlayed = true;

	InitialHeight = GetOwner()->GetActorLocation().Z;
	FinalHeight = InitialHeight + HeightShift;

	// ...
	
}


// Called every frame
void USlideDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ShouldOpen()) {
		OpenDoor(DeltaTime);
	} else {
		CloseDoor(DeltaTime);
	}
}

bool USlideDoor::ShouldOpen() const {
	TArray<AActor*> ActorsInPressurePlate;
	PressurePlate->GetOverlappingActors(ActorsInPressurePlate);

	float TotalMass = 0.f;
	for (AActor* Actor : ActorsInPressurePlate) {
		UPrimitiveComponent* pc = Actor->FindComponentByClass<UPrimitiveComponent>();
		if (pc) {
			TotalMass += pc->GetMass();
		}
	}

	return TotalMass > WeightThreshold;
}


void USlideDoor::OpenDoor(float DeltaTime) {
	//CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetYaw, DeltaTime, 1);
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, FinalHeight, DeltaTime, 1);
	GetOwner()->SetActorLocation(CurrentLocation);

	if(Audio && !OpenSoundHasPlayed) {
		UE_LOG(LogTemp, Warning, TEXT("playing sound in open!!!"));
		Audio->Play();
		OpenSoundHasPlayed = true;
		ClosedSoundHasPlayed = false;
	}
}

void USlideDoor::CloseDoor(float DeltaTime) {
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	CurrentLocation.Z = FMath::FInterpTo(CurrentLocation.Z, InitialHeight, DeltaTime, 2);
	GetOwner()->SetActorLocation(CurrentLocation);

	if(Audio && !ClosedSoundHasPlayed) {
		UE_LOG(LogTemp, Warning, TEXT("playing sound in close!!!"));
		Audio->Play();
		ClosedSoundHasPlayed = true;
		OpenSoundHasPlayed = false;
	}
}



