// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemySpawner.h"
#include "HPcomponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MapLoader.h"
#include "EffectSource.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "TankUnit.h"
#include "Tankogeddon.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComp;

	BuildingMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(SceneComp);

	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->SetupAttachment(SceneComp);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(SceneComp);

	DeathEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Death Effect"));
	DeathEffect->SetupAttachment(SceneComp);

	DeathSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Death Sound"));
	DeathSoundEffect->SetupAttachment(SceneComp);

	HPComp = CreateDefaultSubobject<UHPcomponent>(TEXT("Health component"));
	HPComp->OnDie.AddDynamic(this, &AEnemySpawner::Die);
	HPComp->OnHealthChanged.AddDynamic(this, &AEnemySpawner::DamageTaken);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(SpawningTimerHandle, this, &AEnemySpawner::SpawnUnit, SpawnRate, true, SpawnRate);
}

void AEnemySpawner::TakeDamage(const FDamageData& DamageData)
{
	HPComp->TakeDamage(DamageData);
}

void AEnemySpawner::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(SpawningTimerHandle);
}

void AEnemySpawner::SpawnUnit()
{
	FTransform SpawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1.f));
	ATankUnit* NewTank = GetWorld()->SpawnActorDeferred<ATankUnit>(SpawnClass, SpawnTransform, this, nullptr, 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	NewTank->SetPatrolPoints(TankWaypoints);
	
	NewTank->FinishSpawning(SpawnTransform);
}

void AEnemySpawner::GenerateImpact(FVector& EffectLocation, float Duration)
{
	FTransform SpawnTransform(RootComponent->GetComponentRotation(), RootComponent->GetComponentLocation() + EffectLocation, FVector(1.f));
	AActor* newEffect = GetWorld()->SpawnActor(SmokeEffects, &SpawnTransform);
	if (newEffect)
	{
		Cast<AEffectSource>(newEffect)->ActivateEffects(Duration);
	}
}

void AEnemySpawner::Die()
{
	if (MapLoader)
	{
		MapLoader->CheckGoal();
	}

	if (SmokeSpawnPoints.Num() > 0)
	{
		for (FVector& SmokePoint : SmokeSpawnPoints)
		{
			GenerateImpact(SmokePoint, 15.f);
		}
	}

	HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BuildingMesh->Play(true);

	DeathEffect->ActivateSystem();
	DeathSoundEffect->Play();

	GetWorld()->GetTimerManager().ClearTimer(SpawningTimerHandle);
}

void AEnemySpawner::DamageTaken(float DamageValue)
{
	FVector NewLocation = RootComponent->GetComponentLocation() + FVector(0.f, 0.f, -200.f);
	GenerateImpact(NewLocation, 3.f);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("Factory HP left " + FString::SanitizeFloat(HPComp->GetHPRatio() * 100) + "%"));
}