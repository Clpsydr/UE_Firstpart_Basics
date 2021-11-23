#include "PhysBullet.h"
#include "PhysMovementComponent.h"
#include "Damageable.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

APhysBullet::APhysBullet()
{
	MovementComponent = CreateDefaultSubobject<UPhysMovementComponent>(TEXT("Movement Component"));
}

void APhysBullet::Start()
{
	Super::Start();

	MovementComponent->ReinitValues();
	MovementComponent->SetComponentTickEnabled(true);
}

void APhysBullet::Stop()
{
	MovementComponent->Velocity = FVector::ZeroVector;
	MovementComponent->SetComponentTickEnabled(false);

	Super::Stop();
}

void APhysBullet::Tick(float DeltaSeconds)
{
	float Rotation = Mesh->GetComponentRotation().Roll + 500 * DeltaSeconds;
	Mesh->SetRelativeRotation(FRotator(0.f, 0.f, Rotation));

	if (GetActorLocation().Z < -10000.f)
	{
		Stop();
	}
}

void APhysBullet::AimAt(FVector& NewDirection)
{	
	MovementComponent->TargetLocation = NewDirection;
}

void APhysBullet::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
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

	DrawDebugSphere(GetWorld(), StartPos, 5, 8, FColor::Red, true, 1, 0, 2);
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

			IDamageable* DamageableActor = Cast<IDamageable>(HitActor);
			if (DamageableActor && HitActor != GetWorld()->GetFirstPlayerController()->GetPawn())
			{
				FDamageData DamageData;
				DamageData.DamageValue = Damage;
				DamageData.Instigator = GetOwner();
				DamageData.DamageCause = this;

				DamageableActor->TakeDamage(DamageData);
			}
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
	}

	Stop();
}
