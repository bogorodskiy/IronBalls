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
	KeyboardMoveDirection += FVector{0.0f, 1.0f, 0.0f};
	ProcessMoveInput(KeyboardMoveDirection, true);
}
void ABallPlayerController::OnMoveInputUpReleased()
{
	KeyboardMoveDirection -= FVector{ 0.0f, 1.0f, 0.0f };
	ProcessMoveInput(KeyboardMoveDirection, false);
}
void ABallPlayerController::OnMoveInputRightPressed()
{
	KeyboardMoveDirection += FVector{ 1.0f, 0.0f, 0.0f };
	ProcessMoveInput(KeyboardMoveDirection, true);
}
void ABallPlayerController::OnMoveInputRightReleased()
{
	KeyboardMoveDirection -= FVector{ 1.0f, 0.0f, 0.0f };
	ProcessMoveInput(KeyboardMoveDirection, false);
}
void ABallPlayerController::OnMoveInputDownPressed()
{
	KeyboardMoveDirection += FVector{ 0.0f, -1.0f, 0.0f };
	ProcessMoveInput(KeyboardMoveDirection, true);
}
void ABallPlayerController::OnMoveInputDownReleased()
{
	KeyboardMoveDirection -= FVector{ 0.0f, -1.0f, 0.0f };
	ProcessMoveInput(KeyboardMoveDirection, false);
}
void ABallPlayerController::OnMoveInputLeftPressed()
{
	KeyboardMoveDirection += FVector{ -1.0f, 0.0f, 0.0f };
	ProcessMoveInput(KeyboardMoveDirection, true);
}
void ABallPlayerController::OnMoveInputLeftReleased()
{
	KeyboardMoveDirection -= FVector{ -1.0f, 0.0f, 0.0f };
	ProcessMoveInput(KeyboardMoveDirection, false);
}

void ABallPlayerController::ProcessMoveInput(const FVector& MoveDirection, bool Pressed)
{
	if (ControlledBall != nullptr)
	{	
		auto MovementComponent = ControlledBall->GetBallMovementComponent();
		if (Pressed)
		{
			UE_LOG(LogTemp, Warning, TEXT("KEYBOARD MOVE DIRECTION %s"), *MoveDirection.ToString());
			auto CameraYaw = PlayerCameraManager->GetCameraRotation().Yaw;
			//UE_LOG(LogTemp, Warning, TEXT("CameraYaw %f"), CameraYaw);
			static FVector LookDirection;
			LookDirection.X = FMath::Cos(FMath::DegreesToRadians(CameraYaw));
			LookDirection.Y = FMath::Sin(FMath::DegreesToRadians(CameraYaw));
			LookDirection.Normalize();

			static auto MoveDirectionUp = FVector{ 0.0f, 1.0f, 0.0f };
			auto DotProduct = MoveDirection | MoveDirectionUp;
			auto Determinant = MoveDirection.X * MoveDirectionUp.Y - MoveDirection.Y * MoveDirectionUp.X;
			auto DetAngleRads = FMath::Atan2(Determinant, DotProduct);
			auto DetSin = FMath::Sin(DetAngleRads);
			auto DetCos = FMath::Cos(DetAngleRads);
			static FVector ForceDirection{ 0.0f };
			ForceDirection.X = LookDirection.X * DetCos - LookDirection.Y * DetSin;
			ForceDirection.Y = LookDirection.X * DetSin + LookDirection.Y * DetCos;
			ForceDirection.Normalize();

			MovementComponent->StartApplyForce(ForceDirection);
		}
		else 
		{
			MovementComponent->EndApplyForce();
		}
	}
}

void ABallPlayerController::OnAimAzimuth(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		auto CameraYaw = PlayerCameraManager->GetCameraRotation().Yaw;
		static FVector LookDirection;
		LookDirection.X = FMath::Cos(FMath::DegreesToRadians(CameraYaw));
		LookDirection.Y = FMath::Sin(FMath::DegreesToRadians(CameraYaw));
		//UE_LOG(LogTemp, Warning, TEXT("Look direction: %s"), *LookDirection.ToString());
	}
}

void ABallPlayerController::OnAimElevation(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("-=OnAimElevation: %f=-"), AxisValue);
	}
}