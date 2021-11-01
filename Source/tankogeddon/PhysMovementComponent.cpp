#include "PhysMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UPhysMovementComponent::UPhysMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UPhysMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	ReinitValues();
}

void UPhysMovementComponent::ReinitValues()
{
	float Zstart = GetOwner()->GetActorLocation().Z;
	float Zend = 0.f;

	//applying multiplier of range to velocity
	Velocity = GetOwner()->GetActorForwardVector() * FVector::Distance(GetOwner()->GetActorLocation(), TargetLocation);

	// changing gravity and altitude of velocity to constant
	if (TrajectoryDuration != 0)
	{
		Gravity.Z = 4 * (Zstart - 2 * MaxAltitude + Zend) / (TrajectoryDuration * TrajectoryDuration);
		Velocity.Z = -(3 * Zstart - 4 * MaxAltitude + Zend) / TrajectoryDuration;
	}
}

// Called every frame
void UPhysMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	FVector NewActorLocation = Owner->GetActorLocation() + Velocity * DeltaTime + Gravity * FMath::Square(DeltaTime) / 2.f ;
	Velocity += Gravity * DeltaTime;
	Owner->SetActorLocation(NewActorLocation, true);
}


