// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallPawn.h"
#include "WidgetBlueprintGeneratedClass.h"
#include "GameFramework/PlayerController.h"
#include "BallPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class IRONBALLS_API ABallPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.0f; // 10km in cm

	ABallPawn* GetControlledBall() const;
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SetCrosshairPosition(float AnchorX, float AnchorY);
private:
	float CrosshairAnchorX = 0.0f;
	float CrosshairAnchorY = 0.0f;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void AimTowardsCrosshair();
	bool GetSightRayHitLocation(FVector& OutHitLocation) const;
	bool GetLookHitLocation(FVector StartLocation, FVector LookDirection, FVector& OutHitLocation) const;
};
