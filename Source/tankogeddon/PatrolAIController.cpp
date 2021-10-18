#include "PatrolAIController.h"
#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

void APatrolAIController::BeginPlay()
{
	Super::BeginPlay();

	TankPawn = Cast<ATankPawn>(GetPawn());
	if (TankPawn)
	{
		for (const FVector& Point : TankPawn->GetPatrolPoints())
		{
			AIPatrolPoints.Add(TankPawn->GetActorLocation() + Point);
		}
	}

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void APatrolAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TankPawn && bIsAlive) 
	{
		MoveToNextPoint();
		Targeting();
	}
}

void APatrolAIController::MoveToNextPoint()
{
	if (AIPatrolPoints.Num() == 0)
	{
		return;
	}

	TankPawn->MoveForward(1.f);
	FVector SelfLocation = TankPawn->GetActorLocation();
	FVector CurrentPoint = AIPatrolPoints[CurrentPatrolPointIndex];
	if (FVector::DistSquared(SelfLocation, CurrentPoint) <= FMath::Square(TankPawn->GetMovementPrecision()))
	{
		CurrentPatrolPointIndex++;
		if (!AIPatrolPoints.IsValidIndex(CurrentPatrolPointIndex))
		{
			CurrentPatrolPointIndex = 0;
		}
	}

	FVector TurnDirection = CurrentPoint - SelfLocation;
	TurnDirection.Normalize();
	FVector ForwardDirection = TankPawn->GetActorForwardVector();
	FVector RightDirection = TankPawn->GetActorRightVector();

	DrawDebugLine(GetWorld(), SelfLocation, CurrentPoint, FColor::Green, false, 0.1f, 0, 5);

	//dot product between destination and tank directions allows to figure out the angle between directions, 
	//and therefore understand whether turning should be done to one side or another
	float ForwardAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ForwardDirection, TurnDirection)));
	float RightAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(RightDirection, TurnDirection)));

	float RotationValue = 0;
	if (ForwardAngle > 5.f)
		RotationValue = 1;
	if (RightAngle > 90.f)
		RotationValue = -RotationValue;
	
	TankPawn->RotateRight(RotationValue);
}

void APatrolAIController::Targeting()
{
	if (FVector::DistSquared(PlayerPawn->GetActorLocation(), TankPawn->GetActorLocation()) > FMath::Square(TargetingRange))
	{
		return;
	}

	FHitResult HitResult;
	FVector TraceStart = TankPawn->GetActorLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("AIVisionTrace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false; 
	
	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Blue, false, 0.1f, 0, 5);
		if (HitResult.Actor != PlayerPawn)
		{
			return;
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 0.1f, 0, 5);
	}

	TankPawn->SetTurretTargetAngle(PlayerPawn->GetActorLocation());
	FVector TargetingDirection = TankPawn->GetTurretForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - TankPawn->GetActorLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));

	if (AimAngle <= Accuracy)
	{
		TankPawn->Fire();
	}
}

void APatrolAIController::TurnOff()
{
	bIsAlive = false;
}