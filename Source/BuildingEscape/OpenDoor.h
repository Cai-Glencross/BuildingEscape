// Copyright Cai Glencross 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

private:
	float InitialYaw;
	UPROPERTY(EditAnywhere)
	float TargetYaw = 90.f;
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY(EditAnywhere)
	float MassThreshold = 30.f;
	float DoorLastOpened;
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 2.f;
	UAudioComponent* Audio = nullptr;
	bool OpenSoundHasPlayed;
	bool ClosedSoundHasPlayed;

		
};
