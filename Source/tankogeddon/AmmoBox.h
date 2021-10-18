#pragma once
#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"

UCLASS()
class TANKOGEDDON_API AAmmoBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoBox();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Mesh;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon type")
		ECannonType CannonType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon type")
		int AmmoAmount;

	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//void OnComponentBeginOverlap();
	//void OnMeshBeingOverlap();
};
