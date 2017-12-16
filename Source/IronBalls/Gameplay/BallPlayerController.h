// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BallPawn.h"
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
	ABallPawn* GetControlledBall() const;
	virtual void BeginPlay() override;
	
	
};
