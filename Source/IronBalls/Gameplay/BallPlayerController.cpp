// Fill out your copyright notice in the Description page of Project Settings.

#include "BallPlayerController.h"

ABallPawn* ABallPlayerController::GetControlledBall() const {
	return Cast<ABallPawn>(GetPawn());
}

void ABallPlayerController::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("+++ Ball Player Controller BeginPlay"));
}

