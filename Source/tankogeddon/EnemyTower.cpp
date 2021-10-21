#include "EnemyTower.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyTower::AEnemyTower()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

	HP->OnHealthChanged.AddDynamic(this, &AEnemyTower::OnHealthChanged);
	HP->OnDie.AddDynamic(this, &AEnemyTower::OnDie);
}

void AEnemyTower::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AEnemyTower::Targeting()
{
	if (IsPlayerInRange())
	{
		AimAtPlayer();
		if (CanFire() && AWeaponUnit::Cannon && AWeaponUnit::Cannon->IsReadyToFire())
			Fire();
	}
	else
		Idle();
}

void AEnemyTower::Idle()
{
	FRotator TargetRotation = FRotator(0.f, 0.f, 0.f);
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TargetRotation.Yaw = CurrentRotation.Yaw + 1.f;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
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
	if (FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) > FMath::Square(TargetingRange))
	{
		return false;
	}

	FHitResult HitResult;
	FVector TraceStart = GetActorLocation();
	FVector TraceEnd = PlayerPawn->GetActorLocation();
	FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("TurretVisionTrace")), true, this);
	TraceParams.bReturnPhysicalMaterial = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams))
	{
		if (HitResult.Actor != PlayerPawn)
		{
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
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

void AEnemyTower::OnHealthChanged_Implementation(float Damage)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("Tower HP left " + FString::SanitizeFloat(HP->GetHPRatio() * 100) + "%"));
}

void AEnemyTower::OnDie_Implementation()
{
	DestructionEffect->ActivateSystem();
	DeathSoundEffect->Play();

	PrimaryActorTick.SetTickFunctionEnable(false);
	BodyMesh->SetHiddenInGame(true);
	TurretMesh->SetHiddenInGame(true);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Cannon->Destroy();  // Throws exception, probably because something still attempts to call for the cannon
	SetLifeSpan(3.f);
}