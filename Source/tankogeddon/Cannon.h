#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 3.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		int BurstSize = 3;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float BurstDelay = 0.1f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 1000.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireDamage = 1.f;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ammunition")
		int MachinegunRounds = 300;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Ammunition")
		int LaserBattery = 1000;
		
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		ECannonType FiringType = ECannonType::FireProjectile;


private:
	FTimerHandle ReloadTimerHandle;
	FTimerHandle BurstTimerHandle;
	FTimerDelegate BurstTimer;

	int currentburst = 0;
	bool bIsReadyToFire = false;
	bool bIsBursting = false;

public:	
	// Sets default values for this actor's properties
	ACannon();

	void Fire();
	void AltFire();

	UFUNCTION()
	void Burst(int count);
	void CycleWeapons();

	bool IsOutOfAmmo(ECannonType Type);
	bool IsReadyToFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;  // Old kind of enum that is called through Type!

	void Reload();
};
