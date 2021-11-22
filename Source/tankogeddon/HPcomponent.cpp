// Fill out your copyright notice in the Description page of Project Settings.
#include "HPcomponent.h"

// Sets default values for this component's properties
UHPcomponent::UHPcomponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// WATCH OUT 
	// order between c++ constructor and blueprint values might be improper. headers value will be assumed there before blueprint's changed value
	// CurrentHealth = MaxHealth;
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
