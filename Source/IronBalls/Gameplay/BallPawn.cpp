// Fill out your copyright notice in the Description page of Project Settings.

#include "BallPawn.h"
#include "BallAimingComponent.h"
#include "BallMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Materials/MaterialInterface.h"

ABallPawn::ABallPawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABallPawn::initBallAimingComponent(UBallAimingComponent* value)
{
	if (BallAimingComponent == nullptr)
	{
		BallAimingComponent = value;
	}
}

UBallAimingComponent* ABallPawn::GetBallAimingComponent() const
{
	return BallAimingComponent;
}

void ABallPawn::initBallMovementComponent(UBallMovementComponent* value)
{
	if (BallMovementComponent == nullptr)
	{
		BallMovementComponent = value;
	}
}

UBallMovementComponent* ABallPawn::GetBallMovementComponent() const
{
	return BallMovementComponent;
}

void ABallPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ABallPawn::SetIsBlockingPlayerSight(bool Value)
{
	if (IsBlockingPlayerSight != Value)
	{
		IsBlockingPlayerSight = Value;
		IsChangingAlpha = true;
	}
}

void ABallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsChangingAlpha)
	{
		auto Delta = IsBlockingPlayerSight ? -1.0f : 1.0f;
		auto RootComponent = Cast<UPrimitiveComponent>(GetRootComponent());
		// TODO set opacity
		//RootComponent->GetMaterial()->Opacity
	}
}

void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

