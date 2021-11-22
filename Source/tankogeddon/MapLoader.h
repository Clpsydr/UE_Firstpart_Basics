#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapLoader.generated.h"

UCLASS()
class TANKOGEDDON_API AMapLoader : public AActor
{
	GENERATED_BODY()
	
public:	
	AMapLoader();
    
    UFUNCTION(BlueprintCallable, Category = "MapLoader")
        void CheckGoal();

    UFUNCTION(BlueprintCallable, Category = "MapLoader")
        void SetIsActivated(bool NewIsActivated);

protected:
	virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class UStaticMeshComponent* BodyMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class UStaticMeshComponent* AltBodyMesh;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class UBoxComponent* TriggerCollider;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class UPointLightComponent* ActivatedLight;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
        class UPointLightComponent* DeactivatedLight;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
        FName LoadLevelName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Level loading params")
        bool bIsActivated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level loading params")
        int RequiredFactories = 1;

private:
    void SetActiveLights();

    UFUNCTION()
        void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};







