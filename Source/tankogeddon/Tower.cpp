#include "Tower.h"
#include "TankPController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "HPcomponent.h"
#include "tankogeddon.h"

ATower::ATower()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	//Loading assets from text url
	UStaticMesh* TurretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (TurretMeshTemp)
	{
		TurretMesh->SetStaticMesh(TurretMeshTemp);
	}
	UStaticMesh* BodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (BodyMeshTemp)
	{
		BodyMesh->SetStaticMesh(BodyMeshTemp);
	}

	TowerHP = CreateDefaultSubobject<UHPcomponent>(TEXT("Health component"));
	TowerHP->OnHealthChanged.AddDynamic(this, &ATower::OnHealthChanged);
	TowerHP->OnDie.AddDynamic(this, &ATower::OnDie);
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();

	//Assembling cannon 
	FActorSpawnParameters Params;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, Params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	//Locating player
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void ATower::Destroyed()
{
	Super::Destroyed();

	if (Cannon)
		Cannon->Destroy();
}

void ATower::Targeting()
{
	if (IsPlayerInRange())
		AimAtPlayer();

	if (CanFire() && Cannon && Cannon->IsReadyToFire())
		Fire();
}

void ATower::AimAtPlayer()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator CurrentRotation = TurretMesh->GetComponentRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::RInterpConstantTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), TargetingSpeed));
}

bool ATower::IsPlayerInRange()
{
	return FVector::DistSquared(PlayerPawn->GetActorLocation(), GetActorLocation()) <= FMath::Square(TargetingRange);
}

void ATower::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

bool ATower::CanFire()
{
	FVector TargetingDirection = TurretMesh->GetForwardVector();
	FVector DirectionToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	DirectionToPlayer.Normalize();
	float AimAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TargetingDirection, DirectionToPlayer)));
	return AimAngle <= Accuracy;

}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerPawn)
		Targeting();
}

void ATower::TakeDamage(const FDamageData& DamageData)
{
	TowerHP->TakeDamage(DamageData);
}

void ATower::OnHealthChanged_Implementation(float Damage)
{
	UE_LOG(LogTankgeddon, Log, TEXT("Turret %s is hurt for %f, %f"), *GetName(), Damage);
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("Tower HP left " + FString::SanitizeFloat(TowerHP->GetHPRatio()*100) + "%"));
}

void ATower::OnDie_Implementation()
{
	UE_LOG(LogTankgeddon, Log, TEXT("Turret %s destroyed"), *GetName());
	Destroy();
}