#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "GameStructs.h"
#include "Damageable.h"
#include "HPcomponent.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn, public IDamageable
{
	GENERATED_BODY()

public:
	ATankPawn();

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveForward(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void RotateRight(float Torque);

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void SetTurretTargetAngle(const FVector& TargetDirection);

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void Fire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void AltFire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void WeapChange();

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void RefillAmmo(ECannonType cannontype, int amount);

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UHPcomponent* TankHP;


protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* MainCannonSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* SubCannonSpawnPoint;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* HitCollider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 1500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Motion")
		float TankMotionSmoothness = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Motion")
		float TurretMotionSmoothness = 0.3f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<class ACannon> MainCannonClass;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<class ACannon> SubWeaponClass;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")  
		void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnDie();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class ACannon* Cannon = nullptr;
	UPROPERTY()
		class ACannon* SubWeapon = nullptr;

	class ACannon* ActiveWeapon = nullptr;

	float CurrentForwardAxisValue;
	float CurrentRotationValue;
	float TargetForwardAxisValue;
	float TargetRotationValue;

	FVector TurretTargetAngle;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TakeDamage(const FDamageData& DamageData) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
