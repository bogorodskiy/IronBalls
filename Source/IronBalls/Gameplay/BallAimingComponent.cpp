// Fill out your copyright notice in the Description page of Project Settings.

#include "BallAimingComponent.h"


// Sets default values for this component's properties
UBallAimingComponent::UBallAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UBallAimingComponent::AimAt(FVector Location) {
	//UE_LOG(LogTemp, Warning, TEXT("+++ Tank %s aiming at %s"), *GetName(), *Location.ToString());
}

// Called when the game starts
void UBallAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBallAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

