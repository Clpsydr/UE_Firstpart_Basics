#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Damageable.h"
#include "tankogeddon.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.005f;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnMeshHit);
	RootComponent = Mesh;
}

void AProjectile::OnMeshHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	// bullet hitting its parent shouldn't do anything
	if (OtherActor == GetInstigator())
	{
		return;
	}

	// for destructible environment group
	if (OtherActor && OtherComp && OtherComp->GetCollisionObjectType() == ECC_WorldDynamic)
	{
		OtherActor->Destroy();
	}
	// for interactive objects with expected interfaces to manage health and damage
	else if (IDamageable* Damageable = Cast<IDamageable>(OtherActor))
	{
		FDamageData DmgData;
		DmgData.DamageValue = Damage;
		DmgData.Instigator = GetInstigator();
		DmgData.DamageCause = this;
		Damageable->TakeDamage(DmgData);
	}

	Destroy();
}

void AProjectile::Start()
{
	StartPosition = GetActorLocation();
}

void AProjectile::Stop()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}


void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true); // sweep is true here that causes object to move only until collision

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
	{
		Destroy();
	}
}

