#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/UserDefinedStruct.h"
#include "LevelSpawner.generated.h"

class ARoomSection;
class FObjectPooler;

UENUM()
enum class ERoomType : uint8
{
	Room_Peace,
	Room_Hostile,
	Room_Start,
	Room_End
};

USTRUCT(BlueprintType)
struct FDungeonCell
{
	GENERATED_BODY()

	UPROPERTY()
	FVector2D Pos;					//coordinates in the area
	
	UPROPERTY()
	int8 Directions;				// bitmask for NWES 0000

	UPROPERTY()
	ARoomSection* LinkedRoom;		// connection to uworld assets

	UPROPERTY()
	ERoomType Type;					// type of the room
	FDungeonCell() {}
	FDungeonCell(FVector2D NPos) : Pos(NPos), Type(ERoomType::Room_Hostile) {}
	FDungeonCell(FVector2D NPos, int8 NDir, ERoomType NType) : Pos(NPos), Directions(NDir), Type(NType) {}
};

USTRUCT(BlueprintType)
struct FDungeonField
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDungeonCell> Field;

	UPROPERTY()
	FVector2D StartCoords;

	UPROPERTY()
	FVector2D Dimensions;

	FDungeonField() {}
	FDungeonField(FVector2D NDimension, TArray<FDungeonCell> NField) : Field(NField), Dimensions(NDimension) {}
};

////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class TANKOGEDDON_API ALevelSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelSpawner();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		TSubclassOf<ARoomSection> AssetType;		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation data", meta = (ClampMin = "0", ClampMax="50"))
		int32 TileNumberX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation data", meta = (ClampMin = "0", ClampMax = "50"))
		int32 TileNumberY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generation data", meta = (ClampMin = "0", ClampMax = "1"))
		float SpawnDelay;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//
	FDungeonField InitDungeon(int32 SizeX, int32 SizeY);	 //TODO: add a method of generating the level

	void BasicGeneration(FDungeonField& LinkedDungeon);

	//picks random cell within array bounds
	const FDungeonCell& GetRandomCell(const FDungeonField& LinkedDungeon);

	//???
	void GetRandomDirection(const FDungeonCell& Position, const FVector2D Dimensions);

	//returns list of directions, that aren't OOB or already linked
	int8 GetAllowedDirections(const FDungeonCell& Position, const FVector2D Dimensions);

	//assumes new direction for the room, and optionally for room it links to
	void LinkRoomToDir(FDungeonCell& Room, int8 Direction, bool bIsBidirectional);

	void InitGameLevel(FDungeonField& LinkedDungeon);		 // Actual UE level generation on basis of dungeon data

private:
	FVector2D StartPoint;				// players starting cell

	FDungeonField Dungeon;
	
	TArray<ARoomSection*> RoomObjects;	// at the start I dont need pooling for specifically floor cells, but it would be nice to have one

	FTimerHandle SpawnTimerHandle;
	FTimerDelegate SpawnTDelegate;

	UFUNCTION()
		void SpawnARoom(FDungeonField& ThisDungeon, int32 CellIndex);


	//FObjectPooler FurniturePool;		// to dynamically allocate walls/doors for the tiles


	/// for other map generation methods
	/// lookup generation for wang tiles
};
