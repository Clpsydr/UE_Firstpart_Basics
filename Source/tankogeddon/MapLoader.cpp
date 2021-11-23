#include "MapLoader.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

AMapLoader::AMapLoader()
{
	PrimaryActorTick.bCanEverTick = false;

    USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = SceneComp;

    BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gates Inactive Mesh"));
    BodyMesh->SetupAttachment(SceneComp);

    AltBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gates Active Mesh"));
    AltBodyMesh->SetupAttachment(SceneComp);
    AltBodyMesh->SetVisibility(false, false);

    ActivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Activated lights"));
    ActivatedLight->SetupAttachment(SceneComp);

    DeactivatedLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Deactivated lights"));
    DeactivatedLight->SetupAttachment(SceneComp);

    TriggerCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger collider"));
    TriggerCollider->SetupAttachment(SceneComp);

    TriggerCollider->OnComponentBeginOverlap.AddDynamic(this, &AMapLoader::OnTriggerOverlapBegin);
}

void AMapLoader::BeginPlay()
{
	Super::BeginPlay();
    SetActiveLights();
}

void AMapLoader::CheckGoal()
{
    RequiredFactories--;
    if (RequiredFactories < 1)
    {
        SetIsActivated(true);
    }
}

void AMapLoader::SetIsActivated(bool NewIsActivated)
{
    bIsActivated = NewIsActivated;
    SetActiveLights();

    AltBodyMesh->SetVisibility(NewIsActivated, false);
    BodyMesh->SetVisibility(!NewIsActivated, false);
}

void AMapLoader::SetActiveLights()
{
    ActivatedLight->SetHiddenInGame(!bIsActivated);
    DeactivatedLight->SetHiddenInGame(bIsActivated);
}

void AMapLoader::OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsActivated)
    {
        return;
    }
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (OtherActor == PlayerPawn)
    {
        UGameplayStatics::OpenLevel(GetWorld(), LoadLevelName);
    }

}


