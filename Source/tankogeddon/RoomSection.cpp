#include "RoomSection.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

ARoomSection::ARoomSection()
{
	PrimaryActorTick.bCanEverTick = false;

	FoundationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Foundation"));
	RootComponent = FoundationMesh;

	// anchors temporarily always generate walls, that get hidden on maze generation
	SWallAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SWall"));
	NWallAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NWall"));
	EWallAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EWall"));
	WWallAnchor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WWall"));

	NWallAnchor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	SWallAnchor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	EWallAnchor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	WWallAnchor->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ARoomSection::BeginPlay()
{
	Super::BeginPlay();
	GetActorBounds(false, CenterPos, Dimensions, false);		// temporal until proper boundary data is setup
	
	DynaMaterial = FoundationMesh->CreateDynamicMaterialInstance(0);
}

void ARoomSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARoomSection::SetNewTexColor(FLinearColor NewColor)
{
	DisplayMatColor = NewColor;
	if (DynaMaterial)
	{
		DynaMaterial->SetVectorParameterValue(FName("ColorParam"), DisplayMatColor);
	}
}

void ARoomSection::AssumeWallPassability(const int8 ClipMask)
{
	//needs an array of walls I guess

	//for (ThisBit from 4 to 8 in ClipMask)
	// ThisBit ? {} : HideWall

}

