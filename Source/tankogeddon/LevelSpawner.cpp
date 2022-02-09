#include "LevelSpawner.h"
//#include "Math/UnrealMathUtility.h"

ALevelSpawner::ALevelSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ALevelSpawner::BeginPlay()
{
	Super::BeginPlay();

	Dungeon = InitDungeon(10, 10);

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
		NewDungeon.Field.Add(NewCell); //if setting up connections, apply conditions that dont go outside of sizexy
	}

	return NewDungeon;
}

void ALevelSpawner::BasicGeneration(FDungeonField* LevelSpawner)
{
	// most common algoryth to test things out first
}

void ALevelSpawner::LinkRooms(FDungeonCell RoomA, FDungeonCell RoomB)
{
}

void ALevelSpawner::SetStartingRoom(FVector2D StartPosition)
{
}

void ALevelSpawner::InitGameLevel(FDungeonField* LinkedDungeon)
{
	// picks up local asset example and instantiates blocks regarding to their position and size
	// adds playerstart if cell is of a start type

	// later on adds navigation points and spawners according to other room types
}


