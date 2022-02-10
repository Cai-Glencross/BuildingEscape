// Copyright Cai Glencross 2022.


#include "DisplayLabel.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UDisplayLabel::UDisplayLabel()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDisplayLabel::BeginPlay()
{
	Super::BeginPlay();

	Player = GetWorld()->GetFirstPlayerController();
	Text = GetOwner()->FindComponentByClass<UTextRenderComponent>();

	if(!Text) {
		UE_LOG(LogTemp, Error, TEXT("No Text Render Component on %s"), *GetOwner()->GetName());
	}
	hasDisplayedLabel = false;

	InitialLocation = GetOwner()->GetActorLocation();

	// ...
	
}


// Called every frame
void UDisplayLabel::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!InitialLocation.Equals(GetOwner()->GetActorLocation(), 100)) {
		hasDisplayedLabel = true;
		UE_LOG(LogTemp, Warning, TEXT("lamp has moved"));
	}
	//UE_LOG(LogTemp, Warning, TEXT("Distance to lamp is %f"), (Player->GetPawn()->GetActorLocation() - GetOwner()->GetActorLocation()).Size());
	if (!hasDisplayedLabel && (Player->GetPawn()->GetActorLocation() - GetOwner()->GetActorLocation()).Size() < DistanceThreshold) {
		Text->SetVisibility(true);
		UE_LOG(LogTemp, Warning, TEXT("lamp is visible"));
		Text->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), Player->GetPawn()->GetActorLocation()));
	} else {
		Text->SetVisibility(false);
	}

}

