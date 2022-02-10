// Copyright Cai Glencross 2022.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();	

	SetUpPhysicsHandle();
	SetUpInputComponent();
}

void UGrabber::SetUpPhysicsHandle() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandle found on %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetUpInputComponent() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
	InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LineTraceEnd;
	FVector LineTraceStart;
	GetReachStartAndEnd(OUT LineTraceStart, OUT LineTraceEnd);

	if (PhysicsHandle && PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

FHitResult UGrabber::GetFirstObjectWithinReach() {
	FVector PlayerViewPointLocation;
	FVector LineTraceEnd;
	GetReachStartAndEnd(OUT PlayerViewPointLocation, OUT LineTraceEnd);

	FCollisionQueryParams TraceParams(FName(TEXT("PickupCollision")), false, GetOwner());
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return hit;
}

void UGrabber::GetReachStartAndEnd(FVector& out_ReachStart, FVector& out_ReachEnd) {
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT out_ReachStart, 
		OUT PlayerViewPointRotation
	);
	//Draw a line showing the reach
	out_ReachEnd = out_ReachStart + PlayerViewPointRotation.Vector() * Reach;
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grabbing!!"));

	FHitResult Hit = GetFirstObjectWithinReach();
	
	if (Hit.GetActor() && PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Actor that we hit is %s"), *Hit.GetActor()->GetName());

		PhysicsHandle->GrabComponentAtLocation(
			Hit.GetComponent(), 
			NAME_None, 
			Hit.GetActor()->GetActorLocation()
		);
	}

}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Releasing!!"));
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}
}

