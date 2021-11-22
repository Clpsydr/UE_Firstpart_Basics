#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BulletPoolSubsystem.generated.h"

UCLASS()
class TANKOGEDDON_API UBulletPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	AActor* RetrieveActor(UClass* Class, const FTransform& Transform);
	void ReturnActor(AActor* Actor);
	bool IsActorInPool(AActor* Actor) const;

protected:
	virtual bool DoesSupportWorldType(EWorldType::Type WorldType) const override;

private:
	UFUNCTION()
	void OnActorDestroyed(AActor* Actor);

	UPROPERTY()
	TArray<class AActor*> ActorArray;
	TMap<UClass*, TArray<class AActor*>> InactiveActors;
};

