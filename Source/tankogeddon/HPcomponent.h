#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStructs.h"
#include "HPcomponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, DamageValue);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UHPcomponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHPcomponent(); // Sets default values for this component's properties

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health values")
		float MaxHealth = 20.f;

	UPROPERTY(BlueprintAssignable)
		FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
		FOnDie OnDie;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
		void TakeDamage(const FDamageData& DmgData);

	UFUNCTION(BlueprintCallable, Category = "Health Component")
		float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
		float GetHPRatio() const;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
		void AddHealth(float HPBonus);

protected:
	UPROPERTY()
		float CurrentHealth = 0.f;

	virtual void BeginPlay() override;
};
