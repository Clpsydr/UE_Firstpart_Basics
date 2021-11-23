// Fill out your copyright notice in the Description page of Project Settings.
#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "BulletPoolSubsystem.h"
#include "Damageable.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Camera/CameraShakeBase.h"
#include "GameFramework/ForceFeedbackEffect.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent* SceneCmp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneCmp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Shoot effect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);
	LaserEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Laser effect"));
	LaserEffect->SetupAttachment(ProjectileSpawnPoint);

	FireSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Explosion Sound"));
	FireSoundEffect->SetupAttachment(ProjectileSpawnPoint);
}

void ACannon::Fire()
{
	if (!bIsReadyToFire || IsOutOfAmmo(FiringType) || bIsBursting)
	{
		return;
	}
	bIsReadyToFire = false;
	Ammo--;
	Burst(1, FiringType);
}

void ACannon::AltFire()  //while this method is replaceable by putting burst argument into Fire(), I kept AltFire() around for trying out different logic
{
	if (!bIsReadyToFire || IsOutOfAmmo(FiringType) || bIsBursting)
	{
		return;
	}
	bIsReadyToFire = false;
	Ammo--;

	BurstTimer.BindUFunction(this, FName("Burst"), BurstSize, FiringType);
	GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, BurstTimer, BurstDelay, false);
}

void ACannon::Burst(int Count, ECannonType CurrentFiringType)
{
	if (Count == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
	}
	else
	{
		if (GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
		{
			if (ShootJoypadFeedback)
			{
				FForceFeedbackParameters Params;
				Params.bLooping = false;
				Params.Tag = TEXT("ShootFFParams");
				GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootJoypadFeedback);
			}

			if (ShootCameraFeedback)
			{
				GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(ShootCameraFeedback);
			}
		}

		BurstTimer.BindUFunction(this, FName("Burst"), Count - 1, CurrentFiringType);
		GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, BurstTimer, BurstDelay, false);

		if (CurrentFiringType == ECannonType::FireProjectile)
		{
					UBulletPoolSubsystem* BulletPool = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
			FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
			AProjectile* Projectile = Cast<AProjectile>(BulletPool->RetrieveActor(ProjectileClass, SpawnTransform));
			if (Projectile)
			{
				ShootEffect->ActivateSystem();
				FireSoundEffect->Play();

				Projectile->Start();
			}
		}
		else if (CurrentFiringType == ECannonType::FireTrace)
		{
			FireSoundEffect->Play();

			FHitResult HitResult;
			FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
			FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
			FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
			TraceParams.bReturnPhysicalMaterial = false;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, TraceParams))
			{
				LaserEffect->ActivateSystem();
				LaserEffect->SetBeamSourcePoint(0, TraceStart, 0);
				LaserEffect->SetBeamTargetPoint(0, HitResult.Location, 0);

				if (IDamageable* Damageable = Cast<IDamageable>(HitResult.Actor))
				{
					GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Orange, TEXT("Hit a " + HitResult.Actor->GetName()));
					FDamageData DmgData;
					DmgData.DamageValue = FireDamage;
					DmgData.Instigator = GetInstigator();
					DmgData.DamageCause = this;
					Damageable->TakeDamage(DmgData);
				}
			}
			else
			{
				LaserEffect->ActivateSystem();
				LaserEffect->SetBeamSourcePoint(0, TraceStart, 0);
				LaserEffect->SetBeamTargetPoint(0, TraceEnd, 0);
			}
		}
		else if (CurrentFiringType == ECannonType::FireMortar)
		{
			UBulletPoolSubsystem* BulletPool = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
			FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
			APhysBullet* Projectile = Cast<APhysBullet>(BulletPool->RetrieveActor(ProjectileClass, SpawnTransform));

			FVector DirectionToPlayer = ProjectileSpawnPoint->GetComponentLocation() + FireRange;
			Projectile->AimAt(DirectionToPlayer);

			if (Projectile)
			{
				ShootEffect->ActivateSystem();
				FireSoundEffect->Play();

				Projectile->Start();
			}
		}
	}
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire; 
}

bool ACannon::IsOutOfAmmo(ECannonType CurrentType)
{
	return (Ammo <= 0);
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	bIsReadyToFire = true;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	BurstTimer.Unbind();
	GetWorld()->GetTimerManager().ClearTimer(BurstTimerHandle);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
	bIsBursting = false;
}

void ACannon::Refill(int Amount)
{
	Ammo = FMath::Clamp(Ammo+Amount, Amount, MaxAmmo);
}

ECannonType ACannon::GetType()
{
	return FiringType;
}

/// quite a dumb way to change weapons, maybe I should make sort of a weapon class or the cannon itself should be one
void ACannon::CycleWeapons(ECannonType NewType)
{
	FiringType = NewType;
}
