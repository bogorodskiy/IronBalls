// Fill out your copyright notice in the Description page of Project Settings.

#include "BallPlayerController.h"
#include "BallAimingComponent.h"
#include "BallMovementComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Camera/PlayerCameraManager.h"

void ABallPlayerController::SetCrosshairPosition(float AnchorX, float AnchorY)
{
	CrosshairAnchorX = AnchorX;
	CrosshairAnchorY = AnchorY;
}

void ABallPlayerController::Possess(APawn* PawnToPossess)
{
	Super::Possess(PawnToPossess);
	ControlledBall = Cast<ABallPawn>(GetPawn());
	if (ControlledBall == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ball player controller possessed unknown pawn"));
	}
}

void ABallPlayerController::UnPossess()
{
	Super::UnPossess();
	ControlledBall = nullptr;
}

void ABallPlayerController::BeginPlay() 
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("+++ Ball Player Controller BeginPlay"));
}

void ABallPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// TODO: unbind
	InputComponent->BindAction("MoveUp", IE_Pressed, this, &ABallPlayerController::OnMoveInputUpPressed);
	InputComponent->BindAction("MoveUp", IE_Released, this, &ABallPlayerController::OnMoveInputUpReleased);
	InputComponent->BindAction("MoveRight", IE_Pressed, this, &ABallPlayerController::OnMoveInputRightPressed);
	InputComponent->BindAction("MoveRight", IE_Released, this, &ABallPlayerController::OnMoveInputRightReleased);
	InputComponent->BindAction("MoveDown", IE_Pressed, this, &ABallPlayerController::OnMoveInputDownPressed);
	InputComponent->BindAction("MoveDown", IE_Released, this, &ABallPlayerController::OnMoveInputDownReleased);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &ABallPlayerController::OnMoveInputLeftPressed);
	InputComponent->BindAction("MoveLeft", IE_Released, this, &ABallPlayerController::OnMoveInputLeftReleased);

	InputComponent->BindAxis("AimAzimuth", this, &ABallPlayerController::OnAimAzimuth);
	InputComponent->BindAxis("AimElevation", this, &ABallPlayerController::OnAimElevation);
}

void ABallPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO: only if in battle mode
	AimTowardsCrosshair();
}

void ABallPlayerController::AimTowardsCrosshair() 
{
	if (ControlledBall == nullptr)
	{
		return;
	}
	auto AimingComponent = ControlledBall->GetBallAimingComponent();
	if (AimingComponent != nullptr)
	{
		FVector HitLocation;
		if (GetSightRayHitLocation(HitLocation))
		{
			AimingComponent->AimAt(HitLocation);
		}
	}
}

bool ABallPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const 
{
	static int32 ViewportSizeX = 0;
	static int32 ViewportSizeY = 0;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto CrosshairPositionX = ViewportSizeX * CrosshairAnchorX;
	auto CrosshairPositionY = ViewportSizeY * CrosshairAnchorY;
	static FVector CameraWorldLocation{ 0.0f };
	static FVector CameraLookDirection{ 0.0f };

	auto DeprojectSuccessfull = DeprojectScreenPositionToWorld(CrosshairPositionX, CrosshairPositionY, CameraWorldLocation, CameraLookDirection);
	if (!DeprojectSuccessfull) 
	{
		return false;
	}

	if (GetLookHitLocation(CameraWorldLocation, CameraLookDirection, OutHitLocation)) 
	{
		return true;
	}

	return false;
}

bool ABallPlayerController::GetLookHitLocation(FVector StartLocation, FVector LookDirection, FVector& OutHitLocation) const 
{
	auto RangeVector = LookDirection * LineTraceRange;
	static FHitResult HitResult;
	
	auto GotHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, (StartLocation + RangeVector), ECC_Visibility);
	if (GotHit) 
	{
		OutHitLocation = HitResult.Location;
		return true;
	}
	return false;
}

void ABallPlayerController::OnMoveInputUpPressed()
{
	ProcessMoveInput(MoveInputId::UP, true);
}
void ABallPlayerController::OnMoveInputUpReleased()
{
	ProcessMoveInput(MoveInputId::UP, false);
}
void ABallPlayerController::OnMoveInputRightPressed()
{
	ProcessMoveInput(MoveInputId::RIGHT, true);
}
void ABallPlayerController::OnMoveInputRightReleased()
{
	ProcessMoveInput(MoveInputId::RIGHT, false);
}
void ABallPlayerController::OnMoveInputDownPressed()
{
	ProcessMoveInput(MoveInputId::DOWN, true);
}
void ABallPlayerController::OnMoveInputDownReleased()
{
	ProcessMoveInput(MoveInputId::DOWN, false);
}
void ABallPlayerController::OnMoveInputLeftPressed()
{
	ProcessMoveInput(MoveInputId::LEFT, true);
}
void ABallPlayerController::OnMoveInputLeftReleased()
{
	ProcessMoveInput(MoveInputId::LEFT, false);
}

void ABallPlayerController::ProcessMoveInput(MoveInputId Id, bool Pressed)
{
	if (ControlledBall != nullptr)
	{
		auto MovementComponent = ControlledBall->GetBallMovementComponent();
		auto CameraYaw = PlayerCameraManager->GetCameraRotation().Yaw;
		auto Multiplicator = 5.0f;
		auto MoveVector = FVector{ Multiplicator * FMath::Cos(FMath::DegreesToRadians(CameraYaw)), Multiplicator * FMath::Sin(FMath::DegreesToRadians(CameraYaw)), 0.0f };
		
		if (Pressed)
		{
			MovementComponent->StartApplyForce(MoveVector);
		}
		else {
			MovementComponent->EndApplyForce();
		}
	}
}

void ABallPlayerController::OnAimAzimuth(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("-=OnAimAzimuth: %f=-"), AxisValue);
	}
}

void ABallPlayerController::OnAimElevation(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("-=OnAimElevation: %f=-"), AxisValue);
	}
}