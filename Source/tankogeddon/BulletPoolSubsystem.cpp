#include "BulletPoolSubsystem.h"

UBulletPoolSubsystem::UBulletPoolSubsystem()
{
/*	ProjectileSet.Init(GetWorld()->SpawnActor<AProjectile>(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f)), SetVolume);
	for (auto& Proj : ProjectileSet)
	{
		//AProjectile* newproj;
		//newproj = GetWorld()->SpawnActor<AProjectile>(FVector(0.f,0.f,0.f), FRotator(0.f,0.f,0.f));
		//Proj = newproj;
		if (Proj)
		{
			Proj->Stop();
		}
	}*/
}

UBulletPoolSubsystem::~UBulletPoolSubsystem()
{
}

void UBulletPoolSubsystem::GetActiveBullet(const FTransform& NewPosition)
{
	//go through the list until a viable bullet appears
	// return pointer to it

	//iteration through TArray
	// if iterator is disabled

		// Projectile* Result = <<
		// Result = Transform.GetScale3D() * Result->GetActorScale(); // crutch
		// Result->SetActorTransform(FTransform(Transform.GetRotation(), Transform.GetLocation(), CorrectedScale), false, nullptr, ETeleportType::ResetPhysics);
		// Result->SetActorTickEnabled(true);
		//return Result;

	//in cannon call...
		// UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		//FTransform spawntransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector:OneVector);
		//AProjectile* Projectile = Cast<AProjectile>(Pool->RetrieveActor(ProjectileClass, SpawnTransform));
		//if (Projectile) { Projectile->Start(); }

	// in projectile on death...
	// Stop() instead of Destroy()
	// Stop() is...>
		//PrimaryActorTick.SetTickFunctionEnable(false);
		//Mesh->SetHiddenInGame(true);
		//Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//if its in the pool, like above (special function IsActorInPool(this)
		// return actor , I guess I can just say "disable" or something
		// else destroy (doesnt matter because we dont make extraneous objects

	// Start() means
		// enable tick
		// mesh unhide
		// mesh cpllision enabled to QueryAndPhysics
		// StartPosition = GetActorLocation();
		// 


}