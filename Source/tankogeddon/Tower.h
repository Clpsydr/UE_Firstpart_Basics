#pragma once
#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.h"
#include "Damageable.h"
#include "GameFramework/Actor.h"
#include "HPcomponent.h"
#include "Tower.generated.h"

UCLASS()
class TANKOGEDDON_API ATower : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* CannonSetupPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HitCollider;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UHPcomponent* TowerHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		TSubclassOf<class ACannon> CannonClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 3000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingSpeed = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float Accuracy = 10;

	const FString BodyMeshPath = "StaticMesh'/Game/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";  
	const FString TurretMeshPath = "StaticMesh'/Game/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'"; 

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void Targeting();
	void AimAtPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();
	void Idle();

	UFUNCTION(BlueprintNativeEvent, Category="Health")  //Allows to redefine function in blueprints
	void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
	void OnDie();

private:
	UPROPERTY()
		class ACannon* Cannon;
	UPROPERTY()
		class APawn* PlayerPawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(const FDamageData& DamageData) override;

};
