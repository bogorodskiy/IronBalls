// Fill out your copyright notice in the Description page of Project Settings.

#include "BallMovementComponent.h"
#include "BallPawn.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"



UBallMovementComponent::UBallMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBallMovementComponent::StartApplyForce(const FVector& ForceDirection)
{
	AppliedForceDirection = ForceDirection;
}

void UBallMovementComponent::EndApplyForce()
{
	AppliedForceDirection = {0.0f, 0.0f, 0.0f};
}

void UBallMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	auto BallPawn = Cast<ABallPawn>(GetOwner());
	if (BallPawn != nullptr)
	{
		BallRootComponent = Cast<UPrimitiveComponent>(BallPawn->GetRootComponent());
		if (BallRootComponent != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("BALL ROOT COMPONENT ACQUIRED"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BALL ROOT COMPONENT IS NOT AVAILABLE"));
		}
	}
}

void UBallMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BallRootComponent != nullptr && AppliedForceDirection.Size() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("MOVE DIRECTION %s"), *AppliedForceDirection.ToString());
		static const auto ForceMultiplicator = 10000;
		BallRootComponent->AddForceAtLocation(AppliedForceDirection * ForceMultiplicator, BallRootComponent->GetOwner()->GetTargetLocation());
	}
}

