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

	FVector NewActorLocation = Owner->GetActorLocation() + Velocity * DeltaTime + Gravity * FMath::Square(DeltaTime) / 2.f;
	Velocity += Gravity * DeltaTime;
	Owner->SetActorLocation(NewActorLocation, true);
}


