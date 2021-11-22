#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankChunks.generated.h"

UCLASS()
class TANKOGEDDON_API ATankChunks : public AActor
{
	GENERATED_BODY()

public:	
	ATankChunks();

UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* ChunkA;

UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* ChunkB;

UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Duration")
	float TimeToLive = 5.f;

protected:
	virtual void BeginPlay() override;
};
