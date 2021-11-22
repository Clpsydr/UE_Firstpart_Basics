#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UPhysMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPhysMovementComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
		FVector Gravity = FVector(0.f, 0.f, 980.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
		FVector Velocity = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
		float MaxAltitude = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gravity params")
		float TrajectoryDuration = 3.f;

	FVector TargetLocation;
	void ReinitValues();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
