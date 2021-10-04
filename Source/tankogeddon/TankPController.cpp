// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPController.h"
#include "TankPawn.h"

ATankPController::ATankPController()
{

}

void ATankPController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("FrontMovement", this, &ATankPController::MoveForward);
	InputComponent->BindAxis("StrafeMovement", this, &ATankPController::MoveSideways);
}

void ATankPController::BeginPlay()
{
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
}

void ATankPController::MoveSideways(float AxisValue)
{
	TankPawn->MoveSideways(AxisValue);
}

void ATankPController::MoveForward(float AxisValue)
{
	TankPawn->MoveForward(AxisValue);
}


