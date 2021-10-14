#pragma once
#include "CoreMinimal.h"
#include "WeaponUnit.h"
#include "EnemyTower.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API AEnemyTower : public AWeaponUnit
{
	GENERATED_BODY()
	public:
		AEnemyTower();

		virtual void Tick(float DeltaTime) override;

	protected:
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
			float TargetingRange = 1000;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
			float TargetingSpeed = 0.1f;

		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
			float Accuracy = 10;

		virtual void BeginPlay() override;
		void Targeting();
		void AimAtPlayer();
		bool IsPlayerInRange();
		bool CanFire();

	private:
		UPROPERTY()
			class APawn* PlayerPawn;
};