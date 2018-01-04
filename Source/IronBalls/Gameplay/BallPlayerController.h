// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallPawn.h"
#include "WidgetBlueprintGeneratedClass.h"
#include "GameFramework/PlayerController.h"
#include "BallPlayerController.generated.h"

UCLASS()
class IRONBALLS_API ABallPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.0f; // 10km in cm
	UPROPERTY(EditAnywhere)
	float PossessedAngularDamping = 0.0f;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetCrosshairPosition(float AnchorX, float AnchorY);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Aimed at enemy or lost"))
	void AimEnemy(bool HasEnemy) const;

	virtual void Possess(APawn* PawnToPossess) override;
	virtual void UnPossess() override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	bool InputMoveChanged = false;	
	bool AimedAtEnemy = false;

	float CrosshairAnchorX = 0.0f;
	float CrosshairAnchorY = 0.0f;

	ABallPawn* ControlledBall = nullptr;
	FVector InputMoveDirection = {0.0f, 0.0f, 0.0f};

	void AimTowardsCrosshair();
	bool GetLookHitResult(FVector StartLocation, FVector LookDirection, FHitResult& OutHitResult) const;

	void OnMoveInputUpPressed();
	void OnMoveInputUpReleased();
	void OnMoveInputRightPressed();
	void OnMoveInputRightReleased();
	void OnMoveInputDownPressed();
	void OnMoveInputDownReleased();
	void OnMoveInputLeftPressed();
	void OnMoveInputLeftReleased();
	void ProcessInputMove();

	void OnAimAzimuth(float AxisValue);
	void OnAimElevation(float AxisValue);
};
