#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(Display = "projectile fire"),
	FireTrace = 1 UMETA(Display = "tracing fire"),
	FireMortar = 2 UMETA(Display = "mortar fire")
};

USTRUCT(BlueprintType)  //USTRUCT(BlueprintType)  < possible to use in blueprients, but needs properties to be filled as well
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
		float DamageValue;

	UPROPERTY()
		class AActor* Instigator;

	UPROPERTY()
		class AActor* DamageCause;
};