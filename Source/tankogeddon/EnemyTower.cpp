#include "EnemyTower.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyTower::AEnemyTower()
{

}

void AEnemyTower::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (PlayerPawn)
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 4.f, FColor::White, TEXT("Tower finished beginplay and found player"));
}

void AEnemyTower::Targeting()
{
	if (IsPlayerInRange())
		AimAtPlayer();

	if (CanFire() && AWeaponUnit::Cannon && AWeaponUnit::Cannon->IsReadyToFire())
		Fire();
}

void AEnemyTower::AimAtPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

bool AEnemyTower::IsPlayerInRange()
{
	return FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) <= FMath::Square(TargetingRange);
}

bool AEnemyTower::CanFire()
{
	FVector TargetingDirection = TurretMesh->GetForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));
	return AimAngle <= Accuracy;

}

void AEnemyTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn)
	{
		Targeting();
	}
}