// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BallMovementComponent.generated.h"

class UPrimitiveComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class IRONBALLS_API UBallMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBallMovementComponent();

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartApplyForce(FVector value);

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void EndApplyForce();

private:
	UPrimitiveComponent * BallRoot = nullptr;

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
