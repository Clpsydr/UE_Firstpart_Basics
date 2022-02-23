#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomSection.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UMaterialInstanceDynamic;

UCLASS()
class TANKOGEDDON_API ARoomSection : public AActor
{
	GENERATED_BODY()
		// eventually virtual!
		// base class for other room types ie spawning , starting, exiting, and so
private:	
	ARoomSection();

	UPROPERTY()
		FVector CenterPos {(0.f,0.f,0.f)};

	UPROPERTY()
		FVector Dimensions {(0.f,0.f,0.f)};

	UPROPERTY()
		UMaterialInstanceDynamic* DynaMaterial;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Material parameters")
		FLinearColor DisplayMatColor = FLinearColor(1.f, 1.f, 1.f, 1.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh components")
		UStaticMeshComponent* FoundationMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Anchors")
		UStaticMeshComponent* SWallAnchor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Anchors")
		UStaticMeshComponent* NWallAnchor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Anchors")
		UStaticMeshComponent* EWallAnchor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Anchors")
		UStaticMeshComponent* WWallAnchor;

	UFUNCTION()
		const FVector GetBounds() { return Dimensions; };

	UFUNCTION()
		const FVector GetCenter() { return CenterPos; };

	UFUNCTION()
		void AssumeWallPassability(const int8 ClipMask);

	UFUNCTION()
		void SetNewTexColor(FLinearColor NewColor);
};
