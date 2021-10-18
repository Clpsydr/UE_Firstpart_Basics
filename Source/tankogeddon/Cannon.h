#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Projectile.h"
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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class  UParticleSystemComponent* ShootEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class  UParticleSystemComponent* LaserEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class UAudioComponent* FireSoundEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class UForceFeedbackEffect* ShootJoypadFeedback;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class TSubclassOf<class UCameraShakeBase> ShootCameraFeedback;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (ClampMin = 1), Category = "Fire params") // 
		int BurstSize = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "BurstSize > 1", EditConditionHides), Category = "Fire params") 
		float BurstDelay = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
		float FireDamage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammunition")
		int Ammo = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ammunition")
		int MaxAmmo = 1000;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		ECannonType FiringType = ECannonType::FireProjectile;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "FiringType == ECannonType::FireProjectile", EditConditionHides), 
		Category = "Fire params") 
		TSubclassOf<class AProjectile> ProjectileClass;

	


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
		void Burst(int count, ECannonType currentFiringType);
	UFUNCTION()
		void Refill(int amount);

	void CycleWeapons();

	bool IsOutOfAmmo(ECannonType Type);
	bool IsReadyToFire();
	ECannonType GetType();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;  // Old kind of enum that is called through Type!

	void Reload();
};
