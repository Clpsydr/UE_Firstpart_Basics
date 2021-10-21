#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectSource.generated.h"

UCLASS()
class TANKOGEDDON_API AEffectSource : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectSource();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Effects")
		class UParticleSystemComponent* RepeatedEffect;

	virtual void BeginPlay() override;

public:	
	void ActivateEffects(float TTL); 
};
