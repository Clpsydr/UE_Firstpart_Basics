#pragma once
#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.h"
#include "Damageable.h"
#include "GameFramework/Actor.h"
#include "HPcomponent.h"
#include "WeaponUnit.generated.h"

UCLASS()
class TANKOGEDDON_API AWeaponUnit : public AActor, public IDamageable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponUnit();

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
		class UHPcomponent* HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		TSubclassOf<class ACannon> CannonClass;

	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	virtual void Fire();

	UFUNCTION(BlueprintNativeEvent, Category = "Health")  //Allows to redefine function in blueprints
		void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnDie();

	UPROPERTY()
		class ACannon* Cannon;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(const FDamageData& DamageData) override;

};
