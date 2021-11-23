#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameStructs.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
// Only necessary as metainfo
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class TANKOGEDDON_API IDamageable
{
	GENERATED_BODY()

public:
	virtual void TakeDamage(const FDamageData& DamageData) = 0;
};
