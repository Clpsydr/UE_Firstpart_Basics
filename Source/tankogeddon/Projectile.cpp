#include "Projectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "BulletPoolSubsystem.h"
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

void AProjectile::ApplyDamage(AActor* HitActor)
{
	IDamageable* DamageableActor = Cast<IDamageable>(HitActor);
	if (DamageableActor)
	{
		FDamageData DamageData;
		DamageData.DamageValue = Damage;
		DamageData.Instigator = GetOwner();
		DamageData.DamageCause = this;

		DamageableActor->TakeDamage(DamageData);
	}
}

void AProjectile::ApplyPhysics(AActor* HitActor, AActor* OtherActor)
{
	UPrimitiveComponent* HitMesh = Cast<UPrimitiveComponent>(HitActor->GetRootComponent());
	if (HitMesh)
	{
		if (HitMesh->IsSimulatingPhysics())
		{
			FVector ForceVector = OtherActor->GetActorLocation() - GetActorLocation();
			ForceVector.Normalize();
			HitMesh->AddImpulse(ForceVector * AOEPower, NAME_None, true);
		}
	}
}

void AProjectile::OnMeshHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (OtherActor == GetInstigator()) 	
	{
		return;
	}

	if (bIsAOEEnabled)
	{
		FVector StartPos = GetActorLocation();
		FVector EndPos = StartPos + FVector(0.1f);

		FCollisionShape Shape = FCollisionShape::MakeSphere(AOERadius);
		FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
		Params.AddIgnoredActor(this);
		Params.bTraceComplex = false;
		Params.TraceTag = "AOE Trace";
		TArray<FHitResult> AttackHit;

		FQuat Rotation = FQuat::Identity;

		GetWorld()->DebugDrawTraceTag = "AOE Trace";

		bool bSweepResult = GetWorld()->SweepMultiByChannel
		(
			AttackHit,
			StartPos,
			EndPos,
			Rotation,
			ECollisionChannel::ECC_WorldDynamic,
			Shape,
			Params
		);

		if (bSweepResult)
		{
			for (FHitResult HitResult : AttackHit)
			{
				AActor* HitActor = HitResult.GetActor();
				if (!HitActor)
				{
					continue;
				}

				if (bIsAOEDamaging)
				{
					ApplyDamage(HitActor);
				}

				if (bIsAOEPushing)
				{
					ApplyPhysics(HitActor, OtherActor);
				}
			}
		}

	}
	else
	{
		if (OtherComp->IsSimulatingPhysics())
		{
			FVector Impulse = Mass * MoveSpeed * GetActorForwardVector();
			OtherComp->AddImpulseAtLocation(Impulse, HitResult.ImpactPoint);
		}

		ApplyDamage(OtherActor);
	}

	Stop();
}

void AProjectile::Start()
{
	PrimaryActorTick.SetTickFunctionEnable(true);
	StartPosition = GetActorLocation();
	Mesh->SetHiddenInGame(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AProjectile::Stop()
{
	PrimaryActorTick.SetTickFunctionEnable(false);
	Mesh->SetHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UBulletPoolSubsystem* BulletPool = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
	if (BulletPool->IsActorInPool(this))
	{
		BulletPool->ReturnActor(this);
	}
	else
	{
		Destroy();
	}
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * DeltaTime;
	SetActorLocation(NextPosition, true); // sweep is true here that causes object to move only until collision

	if (FVector::Dist(GetActorLocation(), StartPosition) > FireRange)
	{
		Stop();
	}
}

