#include "AmmoBox.h"
#include "Components/StaticMeshComponent.h"
#include "TankPawn.h"

AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);

}

void AAmmoBox::OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* PlayerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (OtherActor == PlayerPawn)
	{
		PlayerPawn->RefillAmmo(CannonType, AmmoAmount);
		Destroy();
	}
}

