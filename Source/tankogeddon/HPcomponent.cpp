#include "HPcomponent.h"

UHPcomponent::UHPcomponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UHPcomponent::TakeDamage(const FDamageData& DmgData)
{
	float GetHitValue = DmgData.DamageValue;
	CurrentHealth -= GetHitValue;

	if (CurrentHealth <= 0.f)
	{
		if (OnDie.IsBound())
			OnDie.Broadcast();
	}
	else
	{
		if (OnHealthChanged.IsBound())
			OnHealthChanged.Broadcast(GetHitValue);
	}
}

float UHPcomponent::GetHealth() const
{
	return CurrentHealth;
}

float UHPcomponent::GetHPRatio() const
{
	return CurrentHealth / MaxHealth;
}

void UHPcomponent::AddHealth(float HPBonus)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + HPBonus, HPBonus, MaxHealth);
}

void UHPcomponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}
