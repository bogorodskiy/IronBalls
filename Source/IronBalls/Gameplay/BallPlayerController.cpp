// Fill out your copyright notice in the Description page of Project Settings.

#include "BallPlayerController.h"
#include "Engine/World.h"

ABallPawn* ABallPlayerController::GetControlledBall() const {
	return Cast<ABallPawn>(GetPawn());
}

void ABallPlayerController::SetCrosshairPosition(float AnchorX, float AnchorY) {
	CrosshairAnchorX = AnchorX;
	CrosshairAnchorY = AnchorY;
}

void ABallPlayerController::BeginPlay() {
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("+++ Ball Player Controller BeginPlay"));
}

void ABallPlayerController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	AimTowardsCrosshair();
}

void ABallPlayerController::AimTowardsCrosshair() {
	auto ControlledBall = GetControlledBall();
	if (ControlledBall == nullptr) {
		return;
	}

	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation)) {
		//UE_LOG(LogTemp, Warning, TEXT("+++ Look direction: %s"), *HitLocation.ToString());
	}
	
}

bool ABallPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const {

	static int32 ViewportSizeX = 0;
	static int32 ViewportSizeY = 0;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto CrosshairPositionX = ViewportSizeX * CrosshairAnchorX;
	auto CrosshairPositionY = ViewportSizeY * CrosshairAnchorY;
	static FVector CameraWorldLocation{ 0.0f };
	static FVector CameraLookDirection{ 0.0f };

	auto DeprojectSuccessfull = DeprojectScreenPositionToWorld(CrosshairPositionX, CrosshairPositionY, CameraWorldLocation, CameraLookDirection);
	if (!DeprojectSuccessfull) {
		
		return false;
	}

	if (GetLookHitLocation(CameraWorldLocation, CameraLookDirection, OutHitLocation)) {
		return true;
	}

	return false;
}

bool ABallPlayerController::GetLookHitLocation(FVector StartLocation, FVector LookDirection, FVector& OutHitLocation) const {
	auto RangeVector = LookDirection * LineTraceRange;
	static FHitResult HitResult;
	
	auto GotHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, (StartLocation + RangeVector), ECC_Visibility);
	if (GotHit) {
		UE_LOG(LogTemp, Warning, TEXT("+++ GOT HIT LOCATION0: %s"), *HitResult.ImpactPoint.ToString());
	}
	return false;
}