// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallMovementComponent.generated.h"

class UPrimitiveComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IRONBALLS_API UBallMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallMovementComponent();

	void StartApplyForce(const FVector& ForceDirection);
	void EndApplyForce();

private:
	UPrimitiveComponent * BallRoot = nullptr;
	FVector AppliedForceDirection = {0.0f, 0.0f, 0.0f};
	UPrimitiveComponent* BallRootComponent = nullptr;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
