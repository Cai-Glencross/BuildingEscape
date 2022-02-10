// Copyright Cai Glencross 2022.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/PlayerController.h"
#include "DisplayLabel.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UDisplayLabel : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDisplayLabel();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UTextRenderComponent* Text;
	APlayerController* Player;
	UPROPERTY(EditAnywhere)
	float DistanceThreshold = 200.f;
	bool hasDisplayedLabel;
	FVector InitialLocation;

		
};
