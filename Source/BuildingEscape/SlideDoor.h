// Copyright Cai Glencross 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/AudioComponent.h"
#include "SlideDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API USlideDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlideDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);

private:
	UPROPERTY(EditAnywhere)
	float HeightShift;
	UPROPERTY()
	float InitialHeight;
	UPROPERTY()
	float FinalHeight;
	UPROPERTY(EditAnywhere)
	AActor* PressurePlate = nullptr;
	UPROPERTY(EditAnywhere)
	float WeightThreshold;
	UAudioComponent* Audio = nullptr;
	bool OpenSoundHasPlayed;
	bool ClosedSoundHasPlayed;

	bool ShouldOpen() const;
};
