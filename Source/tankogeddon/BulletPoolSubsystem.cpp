#include "BulletPoolSubsystem.h"

AActor* UBulletPoolSubsystem::RetrieveActor(UClass* Class, const FTransform& Transform)
{
	if (!Class)
	{
		return nullptr;
	}

	TArray<AActor*>* Pool = InactiveActors.Find(Class);
	AActor* Result = nullptr;
	if (Pool && Pool->Num() > 0)
	{
		Result = Pool->Pop();
		FVector CorrectedScale = Transform.GetScale3D() * Result->GetActorScale();  //theoretically only managed due to bullet mesh wildly scaling
		Result->SetActorTransform(FTransform(Transform.GetRotation(), Transform.GetLocation(), CorrectedScale), false, nullptr, 
			ETeleportType::ResetPhysics);
		Result->SetActorTickEnabled(true);
	}
	else
	{
		Result = GetWorld()->SpawnActor(Class, &Transform);
		if (!Result)
		{
			return nullptr;
		}

		ActorArray.Add(Result);
		Result->OnDestroyed.AddDynamic(this, &UBulletPoolSubsystem::OnActorDestroyed);
	}

	return Result;
}

void UBulletPoolSubsystem::ReturnActor(AActor* Actor)
{
	if (!Actor || !IsActorInPool(Actor))
	{
		return;
	}

	TArray<AActor*>& Pool = InactiveActors.FindOrAdd(Actor->GetClass());
	if (!Pool.Contains(Actor))
	{
		Actor->SetActorTickEnabled(false);
		Pool.Add(Actor);
	}
}

bool UBulletPoolSubsystem::IsActorInPool(AActor* Actor) const
{
	return ActorArray.Contains(Actor);
}

/// TODO: understand what this method means
bool UBulletPoolSubsystem::DoesSupportWorldType(EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UBulletPoolSubsystem::OnActorDestroyed(AActor* Actor)
{
	check(IsActorInPool(Actor));
	TArray<AActor*>* Pool = InactiveActors.Find(Actor->GetClass());
	if (Pool && Pool->Contains(Actor))
	{
		Pool->Remove(Actor);
	}
	ActorArray.Remove(Actor);
}

