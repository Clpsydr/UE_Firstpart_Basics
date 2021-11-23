#include "TankPController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

ATankPController::ATankPController()
{

}

void ATankPController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("FrontMovement", this, &ATankPController::MoveForward);
	InputComponent->BindAxis("RotationMovement", this, &ATankPController::RotateRight);
	InputComponent->BindAction("NormalFire", IE_Pressed, this, &ATankPController::Fire);
	InputComponent->BindAction("BurstFire", IE_Pressed, this, &ATankPController::AltFire);
	InputComponent->BindAction("WeaponChange", IE_Pressed, this, &ATankPController::WeapChange);
}

void ATankPController::BeginPlay()
{
	Super::BeginPlay();
	
	TankPawn = Cast<ATankPawn>(GetPawn());

	bShowMouseCursor = true;
}

void ATankPController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
		
	FVector WorldMousePosition, WorldMouseDirection;
	DeprojectMousePositionToWorld(WorldMousePosition, WorldMouseDirection);

	if (!TankPawn)
	{
		return;
	}

	FVector TurretTargetDirection = WorldMousePosition - TankPawn->GetActorLocation();
	TurretTargetDirection.Z = 0.f;
	TurretTargetDirection.Normalize();
	FVector TurretTargetPosition = TankPawn->GetActorLocation() + TurretTargetDirection * 1000.f;

	TankPawn->SetTurretTargetAngle(TurretTargetPosition);
}

void ATankPController::MoveForward(float AxisValue)
{
	if (TankPawn)
	{
		TankPawn->MoveForward(AxisValue);
	}
}

void ATankPController::RotateRight(float Torque)
{
	if (TankPawn)
	{
		TankPawn->RotateRight(Torque);
	}
}

void ATankPController::Fire()
{
	if (TankPawn)
	{
		TankPawn->Fire();
	}
}

void ATankPController::AltFire()
{
	if (TankPawn)
	{
		TankPawn->AltFire();
	}
}

void ATankPController::WeapChange()
{
	if (TankPawn)
	{
		TankPawn->WeapChange();
	}
}

