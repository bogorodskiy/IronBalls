// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAIController.h"

ABallPawn* ABallAIController::GetControlledBall() const {
	return Cast<ABallPawn>(GetPawn());
}

void ABallAIController::BeginPlay() {
	Super::BeginPlay();
	
	auto Ball = GetControlledBall();
	if (Ball != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("+++ Ball AI Controller BeginPlay %s"), *Ball->GetName());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("+++ Ball AI Controller BeginPlay, no possessed ball"));
	}
}

