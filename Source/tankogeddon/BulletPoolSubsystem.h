#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Projectile.h"
#include "BulletPoolSubsystem.generated.h"

UCLASS()
class TANKOGEDDON_API UBulletPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
private:
	const int SetVolume = 500;
	TArray<class AProjectile*> ProjectileSet;
	

public:
	UBulletPoolSubsystem();
	~UBulletPoolSubsystem();
	void GetActiveBullet(const FTransform& NewPosition);
};

//TMap<Projectile*, TArray<class Projectile*>> InactiveProjectileSet;
// 
//Mesh->SetHiddenInGame()
// 
//for cycling cannons
//Swap(cannon1, cannon2);
