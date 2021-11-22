#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameStructs.h"
#include "Damageable.generated.h"

// This class does not need to be modified.
// only necessary as metainfo
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKOGEDDON_API IDamageable
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	//UFUNCTION(BlueprintCallable) < cant be inherited, but allows BP
	virtual void TakeDamage(const FDamageData& DamageData) = 0;

	//UFUNCTION(BlueprintImplementableEvent)  < BP enabled and inheritable, cant be easily used in c++ though
	//virtual void TakeDamage(const FDamageData& DamageData) PURE_VIRTUAL();
};
