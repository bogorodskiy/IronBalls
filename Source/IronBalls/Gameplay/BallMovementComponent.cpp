// Fill out your copyright notice in the Description page of Project Settings.

#include "BallMovementComponent.h"
#include "BallPawn.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"



UBallMovementComponent::UBallMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UBallMovementComponent::StartApplyForce(FVector value)
{
	UE_LOG(LogTemp, Warning, TEXT("Start apply force %s"), *value.ToString());
	auto BallPawn = Cast<ABallPawn>(GetOwner());
	if (BallPawn != nullptr)
	{
		BallRoot = Cast<UPrimitiveComponent>(BallPawn->GetRootComponent());
		UE_LOG(LogTemp, Warning, TEXT("Move component owner = "), *BallPawn->GetName());
		BallRoot->AddForceAtLocation(value * 10000, BallPawn->GetTargetLocation());
	}
}

void UBallMovementComponent::EndApplyForce()
{
	UE_LOG(LogTemp, Warning, TEXT("End apply force"));
	if (BallRoot != nullptr)
	{
	
	}

}

void UBallMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBallMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

