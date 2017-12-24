// Fill out your copyright notice in the Description page of Project Settings.

#include "BallPawn.h"
#include "BallAimingComponent.h"
#include "BallMovementComponent.h"

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

void ABallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

