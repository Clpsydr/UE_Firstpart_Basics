#include "WeaponUnit.h"
#include "TankPController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "HPcomponent.h"
#include "tankogeddon.h"

// Sets default values
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

	HP = CreateDefaultSubobject<UHPcomponent>(TEXT("Health component"));
	HP->OnHealthChanged.AddDynamic(this, &AWeaponUnit::OnHealthChanged);
	HP->OnDie.AddDynamic(this, &AWeaponUnit::OnDie);
}

// Called when the game starts or when spawned
void AWeaponUnit::BeginPlay()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	AWeaponUnit::Cannon = GetWorld()->SpawnActor<ACannon>(AWeaponUnit::CannonClass, Params);
	AWeaponUnit::Cannon->AttachToComponent(AWeaponUnit::CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 4.f, FColor::White, TEXT("base class performed beginplay, attached a turret"));
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
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 4.f, FColor::White, TEXT("ticking in base"));
}

void AWeaponUnit::TakeDamage(const FDamageData& DamageData)
{
	HP->TakeDamage(DamageData);
}

void AWeaponUnit::OnHealthChanged_Implementation(float Damage)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("HP left " + FString::SanitizeFloat(HP->GetHPRatio() * 100) + "%"));
}

void AWeaponUnit::OnDie_Implementation()
{
	Destroy();
}


