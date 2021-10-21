#include "EffectSource.h"
#include "Particles/ParticleSystemComponent.h"

AEffectSource::AEffectSource()
{
	PrimaryActorTick.bCanEverTick = true;
	RepeatedEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Effect"));
	RootComponent = RepeatedEffect;
}

void AEffectSource::BeginPlay()
{
	Super::BeginPlay();
}

void AEffectSource::ActivateEffects(float TTL)
{
	RepeatedEffect->ActivateSystem();
	SetLifeSpan(TTL);
}


