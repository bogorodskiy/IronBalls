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

	void SetMoveDirection(const FVector& Value);
	void SetInactiveLinearDamping(float Value);

private:
	bool IsPlaying = false;
	bool HasForceApplied = false;
	UPROPERTY(EditAnywhere)
	float MaxVelocity = 5.0f; // cm/s
	UPROPERTY(EditAnywhere)
	float Acceleration = 2000.0f; // cm/s2
	float InactiveLinearDamping = 0.0f;
	UPrimitiveComponent* BallRoot = nullptr;
	UPrimitiveComponent* BallRootComponent = nullptr;
	FVector MoveDirection = { 0.0f, 0.0f, 0.0f };

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
