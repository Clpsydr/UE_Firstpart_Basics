#include "LevelSpawner.h"
#include "ObjectPooler.h"
#include "RoomSection.h"

ALevelSpawner::ALevelSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelSpawner::BeginPlay()
{
	Super::BeginPlay();

	Dungeon = InitDungeon(TileNumberX, TileNumberY);	

	InitGameLevel(Dungeon);
	BasicGeneration(Dungeon);
}

void ALevelSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FDungeonField ALevelSpawner::InitDungeon(int32 SizeX, int32 SizeY)
{
	FDungeonField NewDungeon;

	for (int i = 0; i < SizeX * SizeY; i++)
	{
		FDungeonCell NewCell(FVector2D(FMath::Abs(i / SizeY), i % SizeY));
		NewDungeon.Field.Add(NewCell); 
	}

	BasicGeneration(NewDungeon);
	return NewDungeon;
}

void ALevelSpawner::BasicGeneration(FDungeonField& LinkedDungeon)
{
	const FDungeonCell* StartingPoint = GetRandomCell(LinkedDungeon);
	LinkedDungeon.StartCoords = StartingPoint->Pos;
	// randomly pick direction if allowed

	//employ binary tree first > either N or E , with startpos being lower left corner!

	//GetAllowedDirections >> if not complete zero
	//Link all directions in the set
	//iterate for cells in each of the directions
	//stop iterating if zero directions available 

	//must be issue with tracking back! GetAllowedDirections should check already linked rooms beside boundaries.
}

const FDungeonCell* ALevelSpawner::GetRandomCell(const FDungeonField& LinkedDungeon)
{
	return &LinkedDungeon.Field[FMath::RandRange(0, LinkedDungeon.Field.Num())];
}

int8 ALevelSpawner::GetAllowedDirections(const FDungeonCell& Position, const FVector2D Dimensions)
{
	//NWES order
	int8 newSet = 0b0000;
	if (Position.Pos.Y > 0)				    //&& !(Position.Directions && 0b1000)
		newSet += 0b1000;
	if (Position.Pos.X > 0)					//&& !(Position.Directions && 0b0100)
		newSet += 0b0100;
	if (Position.Pos.X < Dimensions.X - 1) //&& !(Position.Directions && 0b0010)
		newSet += 0b0010;
	if (Position.Pos.Y < Dimensions.Y - 1) //&& !(Position.Directions && 0b0001)
		newSet += 0b0001;

	return newSet;
}

void ALevelSpawner::LinkRoomToDir(FDungeonCell& Room, int8 Direction, bool bIsBidirectional)
{
	Room.Directions = Direction;

	if (bIsBidirectional)
	{
		//mad stuff you're planning to do with binaries but dont know how
		//NextRoom(Room, Direction)->Directions = Direction.Reverse;
	}
}

void ALevelSpawner::InitGameLevel(FDungeonField& LinkedDungeon)
{
	if (AssetType)
	{
		SpawnTDelegate.BindUFunction(this, FName("SpawnARoom"), LinkedDungeon, 0);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnTDelegate, SpawnDelay, false);
		
		/*for (auto Cell = LinkedDungeon.Field.CreateIterator(); Cell; ++Cell)
		{
			Cell->LinkedRoom = GetWorld()->SpawnActor<ARoomSection>(AssetType, FVector().ZeroVector, FRotator().ZeroRotator);
			Cell->LinkedRoom->SetActorLocation(FVector(Cell->Pos.X * Cell->LinkedRoom->GetBounds().X * 2 - Cell->LinkedRoom->GetBounds().X / 2,
				Cell->Pos.Y * Cell->LinkedRoom->GetBounds().Y * 2 - Cell->LinkedRoom->GetBounds().Y / 2,
				0.f));
		}*/
	}
	else
	{
		GLog->Log(ELogVerbosity::Warning, *FString::Printf(TEXT("Cannot generate dungeon, no assets present")));
	}

	

	

// later on adds navigation points and spawners according to other room types
}

void ALevelSpawner::SpawnARoom(FDungeonField& ThisDungeon, int32 CellIndex)
{
	FDungeonCell* ThisCell = &ThisDungeon.Field[CellIndex];

	ThisCell->LinkedRoom = GetWorld()->SpawnActor<ARoomSection>(AssetType, FVector().ZeroVector, FRotator().ZeroRotator);
	ThisCell->LinkedRoom->SetActorLocation(FVector(ThisCell->Pos.X * ThisCell->LinkedRoom->GetBounds().X * 2 - ThisCell->LinkedRoom->GetBounds().X / 2,
												ThisCell->Pos.Y * ThisCell->LinkedRoom->GetBounds().Y * 2 - ThisCell->LinkedRoom->GetBounds().Y / 2,
											  0.f));

	if (ThisCell->Pos == ThisDungeon.StartCoords)
	{
		ThisCell->LinkedRoom->SetNewTexColor(FLinearColor(1.f, 0.f, 0.f, 1.f));
	}
	else
	{
		ThisCell->LinkedRoom->SetNewTexColor(FLinearColor(0.f, 1.f, 0.f, 1.f));
	}

	if (CellIndex+1 < TileNumberX * TileNumberY)
	{
		SpawnTDelegate.BindUFunction(this, FName("SpawnARoom"), ThisDungeon, CellIndex+1);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnTDelegate, SpawnDelay, false);
	}
}




