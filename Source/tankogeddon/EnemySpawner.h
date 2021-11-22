#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Damageable.h"
#include "GameStructs.h"
#include "EnemySpawner.generated.h"

UCLASS()
class TANKOGEDDON_API AEnemySpawner : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* BuildingDestroyedMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* TankSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class UParticleSystemComponent* DeathEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class UAudioComponent* DeathSoundEffect;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UHPcomponent* HPComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
		TSubclassOf<class ATankUnit> SpawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<class AEffectSource> SmokeEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
		float SpawnRate = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Parameters")
		TArray<class ATargetPoint*> TankWaypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn PArameters | Effects", Meta = (MakeEditWidget = true))
		TArray<FVector> SmokeSpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level Parameters")
		class AMapLoader* MapLoader;


	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

public:	
	virtual void TakeDamage(const FDamageData& DamageData) override;

	UFUNCTION()
		void Die();

	UFUNCTION()
		void DamageTaken(float DamageValue);

	void SpawnUnit();

	void GenerateImpact(FVector& EffectLocation, float Duration);

private:
	FTimerHandle SpawningTimerHandle;
};
