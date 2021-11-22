#include "TankChunks.h"
#include "Kismet/KismetMathLibrary.h"

ATankChunks::ATankChunks()
{
	PrimaryActorTick.bCanEverTick = false;

	ChunkA = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Chunk A"));
	ChunkB = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Chunk B"));
	RootComponent = ChunkA;
}

void ATankChunks::BeginPlay()
{
	Super::BeginPlay();
	ChunkA->SetVisibility(false);
	ChunkB->SetVisibility(false);

	TArray<UStaticMeshComponent*> chunks = { ChunkA, ChunkB };
	int randpick = FMath::Rand() % chunks.Num();
	chunks[randpick]->SetVisibility(true);
	SetLifeSpan(TimeToLive);
}

