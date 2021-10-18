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

void ACannon::AltFire()
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

void ACannon::Burst(int count, ECannonType currentFiringType)
{
	if (count == 0)
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

		BurstTimer.BindUFunction(this, FName("Burst"), count - 1, currentFiringType);
		GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, BurstTimer, BurstDelay, false);

		if (currentFiringType == ECannonType::FireProjectile)
		{
			ShootEffect->ActivateSystem();
			FireSoundEffect->Play();

			UBulletPoolSubsystem* BulletPool = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
			FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
			AProjectile* Projectile = Cast<AProjectile>(BulletPool->RetrieveActor(ProjectileClass, SpawnTransform));
			if (Projectile)
			{
				Projectile->Start();
			}
		}
		else if (currentFiringType == ECannonType::FireTrace)
		{
			FireSoundEffect->Play();

			FHitResult HitResult;
			FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
			FVector TraceEnd = ProjectileSpawnPoint->GetComponentLocation() + ProjectileSpawnPoint->GetForwardVector() * FireRange;
			FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
			TraceParams.bReturnPhysicalMaterial = false;
			if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, TraceParams))
			{
				LaserEffect->SetBeamSourcePoint(0, TraceStart, 0);
				LaserEffect->SetFloatParameter("Length", FVector::DistSquared(TraceStart, HitResult.Location));
				LaserEffect->SetBeamEndPoint(0, HitResult.Location);
				LaserEffect->SetBeamTargetPoint(0, HitResult.Location, 0);
				LaserEffect->ActivateSystem();
				//DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5.f);

				if (HitResult.Actor.IsValid() && HitResult.Component.IsValid(), HitResult.Component->GetCollisionObjectType() == ECC_WorldDynamic)
				{
					GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Orange, TEXT("Traced a destructible prop"));
					HitResult.Actor->Destroy();
				}
				else if (IDamageable* Damageable = Cast<IDamageable>(HitResult.Actor))
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
				LaserEffect->SetBeamSourcePoint(0, TraceStart, 0);
				LaserEffect->SetFloatParameter("Length", FVector::DistSquared(TraceStart, TraceEnd));
				LaserEffect->SetBeamEndPoint(0,TraceEnd);
				LaserEffect->SetBeamTargetPoint(0, TraceEnd, 0);
				LaserEffect->ActivateSystem();
				//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
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

// Called when the game starts or when spawned
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

void ACannon::Refill(int amount)
{
	Ammo = FMath::Clamp(Ammo+amount, amount, MaxAmmo);
}

ECannonType ACannon::GetType()
{
	return FiringType;
}

/// quite a dumb way to change weapons, maybe I should make sort of a weapon class or the cannon itself should be one
void ACannon::CycleWeapons()
{
	if (FiringType == ECannonType::FireProjectile)
	{
		FiringType = ECannonType::FireTrace;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("changed to laser"));
	}
	else
	{
		FiringType = ECannonType::FireProjectile;
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("changed to projectiles"));
	}
}
