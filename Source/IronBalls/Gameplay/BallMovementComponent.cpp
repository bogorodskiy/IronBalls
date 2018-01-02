// Fill out your copyright notice in the Description page of Project Settings.

#include "BallMovementComponent.h"
#include "BallPawn.h"
#include "Components/PrimitiveComponent.h"
//#include "Materials/MaterialInterface.h"
//#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Engine/World.h"



UBallMovementComponent::UBallMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBallMovementComponent::SetMoveDirection(const FVector& Value)
{
	MoveDirection = Value;
	HasForceApplied = (MoveDirection.SizeSquared() > 0);
}

void UBallMovementComponent::SetInactiveLinearDamping(float Value)
{
	InactiveLinearDamping = Value;
	if (IsPlaying && BallRootComponent != nullptr)
	{
		BallRootComponent->SetLinearDamping(InactiveLinearDamping);
	}
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
			BallRootComponent->SetLinearDamping(InactiveLinearDamping);
			UE_LOG(LogTemp, Warning, TEXT("BALL ROOT COMPONENT ACQUIRED"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("BALL ROOT COMPONENT IS NOT AVAILABLE"));
		}
	}
	IsPlaying = true;
}

void UBallMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BallRootComponent != nullptr && HasForceApplied)
	{
		UE_LOG(LogTemp, Warning, TEXT("World time = %f"), GetWorld()->TimeSeconds);
		UE_LOG(LogTemp, Warning, TEXT("Delta time = %f"), DeltaTime);
		auto CurrentLinearVelocity = BallRootComponent->GetPhysicsLinearVelocity();
		UE_LOG(LogTemp, Warning, TEXT("SPEED = %f"), CurrentLinearVelocity.Size());
		UE_LOG(LogTemp, Warning, TEXT("POSITION X = %f"), BallRootComponent->GetOwner()->GetActorLocation().X);
		auto CurrentAcceleration = Acceleration;
		auto VelocityDotProduct = CurrentLinearVelocity | MoveDirection;
		if (VelocityDotProduct < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("ADD EXTRA ACCELERATION"));
			CurrentAcceleration += Acceleration;
		}
		auto AccelerationVelocity = CurrentAcceleration * DeltaTime * MoveDirection;
		auto NextLinearSpeed = CurrentLinearVelocity + AccelerationVelocity;
		//UE_LOG(LogTemp, Warning, TEXT("NEXT RAW SPEED = %f"), NextLinearSpeed.Size());
		auto VelocityChange = NextLinearSpeed.Size() / MaxVelocity;
		if (VelocityChange > 1.0f)
		{
			AccelerationVelocity = (NextLinearSpeed / VelocityChange) - CurrentLinearVelocity;
		}
		//UE_LOG(LogTemp, Warning, TEXT("NEXT FIXED SPEED = %f"), (CurrentLinearVelocity + AccelerationVelocity).Size());
		BallRootComponent->AddImpulse(BallRootComponent->GetMass() * AccelerationVelocity);
		UE_LOG(LogTemp, Warning, TEXT("Apply Impulse %s"), *(BallRootComponent->GetMass() * AccelerationVelocity).ToString());
		UE_LOG(LogTemp, Warning, TEXT("-----------------"));
		//BallRootComponent->GetMaterial()->GetPhysicalMaterial()->Friction

		// TODO: optimize, GetSize to GetSquaredSize, if max velocity -> return
		// FIXME: if add fixed acceleration, next velocity is still > MaxVelocity
	}
}

void UBallMovementComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	IsPlaying = false;
}