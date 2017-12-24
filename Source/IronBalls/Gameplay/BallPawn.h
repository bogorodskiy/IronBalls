// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BallPawn.generated.h"

class UBallAimingComponent;
class UBallMovementComponent;

UCLASS()
class IRONBALLS_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void initBallAimingComponent(UBallAimingComponent* value);
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	UBallAimingComponent* GetBallAimingComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void initBallMovementComponent(UBallMovementComponent* value);
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	UBallMovementComponent* GetBallMovementComponent() const;

private:
	UBallAimingComponent* BallAimingComponent = nullptr;
	UBallMovementComponent* BallMovementComponent = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
