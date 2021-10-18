#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "PatrolAIController.generated.h"

UCLASS()
class TANKOGEDDON_API APatrolAIController : public AAIController
{
	GENERATED_BODY()

	public:
		void TurnOff();

	protected:
		UPROPERTY()
			class ATankPawn* TankPawn;
			class APawn* PlayerPawn;

			int32 CurrentPatrolPointIndex = 0;
			TArray<FVector> AIPatrolPoints;

			UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
				float TargetingRange = 3000.f;

			UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
				float Accuracy = 10.f;

	protected:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;
		void MoveToNextPoint();
		void Targeting();

	private:
		bool bIsAlive = true;
};
