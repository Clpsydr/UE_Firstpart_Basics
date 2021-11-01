#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "PhysBullet.generated.h"

UCLASS()
class TANKOGEDDON_API APhysBullet : public AProjectile
{
	GENERATED_BODY()
	
public:
	APhysBullet();

	virtual void Start() override;
	virtual void Stop() override;
	virtual void Tick(float DeltaSeconds) override;
	void AimAt(FVector& NewDirection);

protected:
	virtual void OnMeshHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult) override;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UPhysMovementComponent* MovementComponent;
};
