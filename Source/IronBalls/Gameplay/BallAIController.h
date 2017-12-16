// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BallPawn.h"
#include "BallAIController.generated.h"

/**
 * 
 */
UCLASS()
class IRONBALLS_API ABallAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABallPawn* GetControlledBall() const;
	virtual void BeginPlay() override;
	
	
	
};
