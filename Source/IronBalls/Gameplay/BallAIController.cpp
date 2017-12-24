// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAIController.h"
#include "Engine/World.h"

ABallPawn* ABallAIController::GetControlledBall() const 
{
	return Cast<ABallPawn>(GetPawn());
}

void ABallAIController::BeginPlay() 
{
	Super::BeginPlay();
	
	auto Ball = GetControlledBall();
	if (Ball != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("+++ Ball AI Controller BeginPlay %s"), *Ball->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("+++ Ball AI Controller BeginPlay, no possessed ball"));
	}
	auto PlayerBall = GetPlayerBall();
	if (PlayerBall != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("+++		GetPlayerBall %s"), *PlayerBall->GetName());
	}
}

ABallPawn* ABallAIController::GetPlayerBall() const
{
	auto FirstPlayerController = GetWorld()->GetFirstPlayerController();
	if (FirstPlayerController != nullptr) {
		auto BallPawn = Cast<ABallPawn>(FirstPlayerController->GetPawn());
		return BallPawn;
	}

	return nullptr;
}

