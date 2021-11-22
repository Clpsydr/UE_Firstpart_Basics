#pragma once
#include "CoreMinimal.h"
#include "GameStructs.h"
#include "WeaponUnit.h"
#include "TankUnit.generated.h"

class UStaticMeshComponent;

UCLASS()
class TANKOGEDDON_API ATankUnit : public AWeaponUnit
{
	GENERATED_BODY()
	
public:
	ATankUnit();

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void MoveForward(float AxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void RotateRight(float Torque);

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void SetTurretTargetAngle(const FVector& TargetDirection);

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void AltFire();

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void WeapChange();

	UFUNCTION(BlueprintCallable, Category = "Turret")
		void RefillAmmo(ECannonType cannontype, int amount);

	UFUNCTION(BlueprintPure, Category = "Turret")
		FVector GetTurretForwardVector();

	UFUNCTION(BlueprintCallable, Category = "AI|Movement|Patrolling")
		void SetPatrolPoints(const TArray<class ATargetPoint*>& SetPoints)
	{
		PatrolPoints = SetPoints;
	}

	UFUNCTION(BlueprintPure, Category = "AI|Movement|Patrolling")
		const TArray<class ATargetPoint*>& GetPatrolPoints() const
	{
		return PatrolPoints;
	};

	UFUNCTION(BlueprintPure, Category = "AI|Movement|Patrolling")
		float GetMovementPrecision()
	{
		return MovementPrecision;
	};

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* SubCannonSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 1500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 200.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Motion")
		float TankMotionSmoothness = 500.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Motion")
		float TurretMotionSmoothness = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Movement", Meta = (MakeEditWidget = true))
		TArray<class ATargetPoint*> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Movement")
		float MovementPrecision = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
		TSubclassOf<class ACannon> SubWeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		TSubclassOf<class AAmmoBox> ItemDrop;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		TSubclassOf<class ATankChunks> PostDeathFragments;

	UFUNCTION(BlueprintNativeEvent, Category = "Health")  //Allows to redefine function in blueprints
		void OnHealthChanged(float Damage);

	UFUNCTION(BlueprintNativeEvent, Category = "Health")
		void OnDie();

	virtual void BeginPlay() override;

private:
	UPROPERTY()
		class ACannon* SubWeapon = nullptr;

	class ACannon* ActiveWeapon = nullptr;

	float CurrentForwardAxisValue;
	float CurrentRotationValue;
	float TargetForwardAxisValue;
	float TargetRotationValue;
	
	int DestroyEffectNumber = 6;

	FVector TurretTargetAngle;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	void ChunkGeneration(FVector HitDirection, float HitPower);

};
