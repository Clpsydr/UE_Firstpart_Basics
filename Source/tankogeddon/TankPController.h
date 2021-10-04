// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPController.generated.h"

class ATankPawn;

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPController : public APlayerController
{
	GENERATED_BODY()

	protected:
		UPROPERTY()
			ATankPawn* TankPawn;

	public:
		ATankPController();
		virtual void SetupInputComponent() override;

	protected:
		virtual void BeginPlay() override;
		void MoveForward(float AxisValue);
		void MoveSideways(float AxisValue);
};