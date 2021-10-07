#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "GameStructs.generated.h"

UENUM(BlueprintType)
enum class ECannonType : uint8
{
	FireProjectile = 0 UMETA(Display = "projectile fire"),
	FireTrace = 1 UMETA(Display = "tracing fire"),
	FireCharge = 2 UMETA(Display = "charging fire")
};

