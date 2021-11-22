#include "WeaponUnit.h"
#include "TankPController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "HPcomponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "tankogeddon.h"

AWeaponUnit::AWeaponUnit()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("unit body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("unit turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	DamageEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Damage Effect"));
	DamageEffect->SetupAttachment(BodyMesh);

	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Death Effect"));
	DestructionEffect->SetupAttachment(BodyMesh);

	DeathSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Death Sound"));
	DeathSoundEffect->SetupAttachment(BodyMesh);

	HP = CreateDefaultSubobject<UHPcomponent>(TEXT("Health component"));
}

// Called when the game starts or when spawned
void AWeaponUnit::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;
	AWeaponUnit::Cannon = GetWorld()->SpawnActor<ACannon>(AWeaponUnit::CannonClass, Params);
	AWeaponUnit::Cannon->AttachToComponent(AWeaponUnit::CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AWeaponUnit::Destroyed()
{
	Super::Destroyed();

	if (Cannon)
		Cannon->Destroy();
}

void AWeaponUnit::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

// Called every frame
void AWeaponUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponUnit::TakeDamage(const FDamageData& DamageData)
{
	DamageEffect->ActivateSystem();
	HP->TakeDamage(DamageData);
}

