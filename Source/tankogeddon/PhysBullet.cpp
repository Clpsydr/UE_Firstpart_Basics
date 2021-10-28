// Fill out your copyright notice in the Description page of Project Settings.
#include "PhysBullet.h"
#include "PhysMovementComponent.h"
#include "Damageable.h"
#include "Components/PrimitiveComponent.h"

APhysBullet::APhysBullet()
{
	MovementComponent = CreateDefaultSubobject<UPhysMovementComponent>(TEXT("Movement Component"));
}

void APhysBullet::Start()
{
	Super::Start();

	MovementComponent->Velocity = GetActorForwardVector() * MoveSpeed;
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
	float Rotation = GetActorRotation().Roll + 500 * DeltaSeconds;
	SetActorRotation(FRotator(0.f, 0.f, Rotation));

	if (GetActorLocation().Z < -10000.f)
	{
		Stop();
	}
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

	//GetWorld()->DebugDrawTraceTag = "AOE Trace";

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
