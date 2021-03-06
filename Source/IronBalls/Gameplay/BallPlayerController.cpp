// Fill out your copyright notice in the Description page of Project Settings.

#include "BallPlayerController.h"
#include "BallPawn.h"
#include "BallMovementComponent.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Camera/PlayerCameraManager.h"

// TODO: delete aiming component

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
	ControlledBall->GetBallMovementComponent()->SetAngularDamping(PossessedAngularDamping);
}

void ABallPlayerController::UnPossess()
{
	Super::UnPossess();
	if (ControlledBall != nullptr)
	{
		ControlledBall->GetBallMovementComponent()->SetAngularDamping(0.0f);
	}
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
	if (InputMoveChanged)
	{
		InputMoveChanged = false;
		ProcessInputMove();
	}
}

void ABallPlayerController::AimTowardsCrosshair() 
{
	if (ControlledBall == nullptr)
	{
		return;
	}

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
		return;
	}

	static FHitResult HitResult;
	if (GetLookHitResult(CameraWorldLocation, CameraLookDirection, HitResult))
	{
		auto HitBall = Cast<ABallPawn>(HitResult.Actor.Get());
		if (HitBall != nullptr && HitBall != ControlledBall)
		{
			if (!AimedAtEnemy)
			{
				// TODO: Move to aim component
				// TODO: Aim at pawn in front of me?
				AimedAtEnemy = true;
				this->DispatchAimEnemyEvent(true);
			}
			return;
		}
	}

	if (AimedAtEnemy)
	{
		AimedAtEnemy = false;
		this->DispatchAimEnemyEvent(false);
	}
}

bool ABallPlayerController::GetLookHitResult(FVector StartLocation, FVector LookDirection, FHitResult& OutHitResult) const
{
	auto RangeVector = LookDirection * LineTraceRange;
	static TArray<FHitResult> HitResults;
	auto GotHitObjects = GetWorld()->LineTraceMultiByChannel(HitResults, StartLocation, (StartLocation + RangeVector), ECC_Pawn);
	if (GotHitObjects)
	{
		auto HitSomething = false;
		for (int i = HitResults.Num() - 1; i >= 0; --i)
		{
			auto HitResult = HitResults[i];
			if (HitResult.Actor != ControlledBall)
			{
				OutHitResult = HitResult;
				HitSomething = true;
			}
			else 
			{
				break;
			}
		}
		return HitSomething;
	}
	return false;
}

void ABallPlayerController::OnMoveInputUpPressed()
{
	InputMoveDirection += FVector{0.0f, 1.0f, 0.0f};
	InputMoveChanged = true;
}
void ABallPlayerController::OnMoveInputUpReleased()
{
	InputMoveDirection -= FVector{ 0.0f, 1.0f, 0.0f };
	InputMoveChanged = true;
}
void ABallPlayerController::OnMoveInputRightPressed()
{
	InputMoveDirection += FVector{ 1.0f, 0.0f, 0.0f };
	InputMoveChanged = true;
}
void ABallPlayerController::OnMoveInputRightReleased()
{
	InputMoveDirection -= FVector{ 1.0f, 0.0f, 0.0f };
	InputMoveChanged = true;
}
void ABallPlayerController::OnMoveInputDownPressed()
{
	InputMoveDirection += FVector{ 0.0f, -1.0f, 0.0f };
	InputMoveChanged = true;
}
void ABallPlayerController::OnMoveInputDownReleased()
{
	InputMoveDirection -= FVector{ 0.0f, -1.0f, 0.0f };
	InputMoveChanged = true;
}
void ABallPlayerController::OnMoveInputLeftPressed()
{
	InputMoveDirection += FVector{ -1.0f, 0.0f, 0.0f };
	InputMoveChanged = true;
}
void ABallPlayerController::OnMoveInputLeftReleased()
{
	InputMoveDirection -= FVector{ -1.0f, 0.0f, 0.0f };
	InputMoveChanged = true;
}
	
void ABallPlayerController::ProcessInputMove()
{
	if (ControlledBall == nullptr)
	{
		return;
	}

	auto MovementComponent = ControlledBall->GetBallMovementComponent();
	static FVector MoveDirection{ 0.0f };
	MoveDirection = {0.0f, 0.0f, 0.0f};
	//UE_LOG(LogTemp, Warning, TEXT("KEYBOARD MOVE DIRECTION %s"), *InputMoveDirection.ToString());
	if (InputMoveDirection.SizeSquared() > 0.0f)
	{
		auto CameraYaw = PlayerCameraManager->GetCameraRotation().Yaw;
		//UE_LOG(LogTemp, Warning, TEXT("CameraYaw %f"), CameraYaw);
		static FVector LookDirection;
		FMath::SinCos(&LookDirection.Y, &LookDirection.X, FMath::DegreesToRadians(CameraYaw));
		LookDirection.Normalize();

		static const auto InputDirectionUp = FVector{ 0.0f, 1.0f, 0.0f };
		auto DotProduct = InputMoveDirection | InputDirectionUp;
		auto Determinant = InputMoveDirection.X * InputDirectionUp.Y - InputMoveDirection.Y * InputDirectionUp.X;
		auto DetAngleRads = FMath::Atan2(Determinant, DotProduct);
		auto DetSin = 0.0f;
		auto DetCos = 0.0f;
		FMath::SinCos(&DetSin, &DetCos, DetAngleRads);

		MoveDirection.X = LookDirection.X * DetCos - LookDirection.Y * DetSin;
		MoveDirection.Y = LookDirection.X * DetSin + LookDirection.Y * DetCos;
		MoveDirection.Normalize();
	}

	//UE_LOG(LogTemp, Warning, TEXT("FORCE DIRECTION %s \n"), *MoveDirection.ToString());
	MovementComponent->SetMoveDirection(MoveDirection);
}

void ABallPlayerController::OnAimAzimuth(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		InputMoveChanged = true;
	}
}

void ABallPlayerController::OnAimElevation(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		//UE_LOG(LogTemp, Warning, TEXT("-=OnAimElevation: %f=-"), AxisValue);
	}
}

//void ABallPlayerController::AimEnemy(bool HasEnemy)
//{
//
//}