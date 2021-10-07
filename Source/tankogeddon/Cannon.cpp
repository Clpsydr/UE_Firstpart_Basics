// Fill out your copyright notice in the Description page of Project Settings.
#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneCmp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneCmp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);

}

void ACannon::Fire()
{
	if (!bIsReadyToFire || IsOutOfAmmo(FiringType) || bIsBursting)
	{
		return;
	}
	bIsReadyToFire = false;

	if (FiringType == ECannonType::FireProjectile)
	{
		//GEngine is a singleton.
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Orange, TEXT("firing a projectile " + FString::FromInt(MachinegunRounds)));
		MachinegunRounds--;
	}
	else if (FiringType == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("firing a laser " + FString::FromInt(LaserBattery)));
		LaserBattery--;
	}

	// timer function is bound to current world 
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1.f / FireRate, false);
}

void ACannon::AltFire()
{
	if (!bIsReadyToFire || IsOutOfAmmo(FiringType) || bIsBursting)
	{
		return;
	}
	bIsReadyToFire = false;
	//need to set current burst value once / along with subtracting the 

	if (FiringType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Orange, TEXT("projectile burst start " + FString::FromInt(MachinegunRounds)));
		MachinegunRounds--;
	}
	else if (FiringType == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("laser burst start " + FString::FromInt(LaserBattery)));
		LaserBattery--;
	}
	
	BurstTimer.BindUFunction(this, FName("Burst"), BurstSize);
	GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, BurstTimer, BurstDelay, false);
}

void ACannon::Burst(int count)
{
	if (count == 0)
	{
		//bIsReadyToFire = false;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, BurstDelay, false);
	}
	else
	{
		BurstTimer.BindUFunction(this, FName("Burst"), count - 1);
		GetWorld()->GetTimerManager().SetTimer(BurstTimerHandle, BurstTimer, BurstDelay, false);

		if (FiringType == ECannonType::FireProjectile)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Orange, TEXT("projectile n " + FString::FromInt(BurstSize- count + 1)));
		}
		else if (FiringType == ECannonType::FireTrace)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Red, TEXT("laser beam n " + FString::FromInt(BurstSize - count + 1)));
		}
	}
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire; 
}

bool ACannon::IsOutOfAmmo(ECannonType CurrentType)
{
	if (CurrentType == ECannonType::FireProjectile)
		return (MachinegunRounds <= 0);
	else if (CurrentType == ECannonType::FireTrace)
		return (LaserBattery <= 0);
	return false;
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
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
	bIsBursting = false;
}

/// quite a dumb way to change weapons, maybe I should make sort of a looped container
void ACannon::CycleWeapons()
{
	if (!bIsBursting)
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
}

