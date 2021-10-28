#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKOGEDDON_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();
	virtual void Tick(float DeltaSeconds) override;
	virtual void Start();
	virtual void Stop(); 

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float MoveSpeed = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
		float FireRange = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float Damage = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
		float Mass = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
		bool bIsAOEPushing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
		bool bIsAOEDamaging;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
		bool bIsAOEEnabled;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
		float AOERadius = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Physics")
		float AOEPower = 200.f;

	UFUNCTION()
		virtual void OnMeshHit(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

		void ApplyDamage(AActor* HitActor);
		void ApplyPhysics(AActor* HitActor, AActor* OtherActor);

private:
	FVector StartPosition;

	

};
