#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/UserDefinedStruct.h"
#include "LevelSpawner.generated.h"

class ARoomAsset;

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
	FVector2D Pos; //coordinates in the area
	
	UPROPERTY()
	int8 Directions;  // bitmask for NWES 0000

	UPROPERTY()
	ERoomType Type;		// type of the room, has to be replaced by enum

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
	FVector2D Dimensions();
};

////////////////////////////////////////////////////////////////////////////////////////////////////
UCLASS()
class TANKOGEDDON_API ALevelSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	ALevelSpawner();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components")
		TSubclassOf<ARoomAsset> AssetType;		

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//
	FDungeonField InitDungeon(int32 SizeX, int32 SizeY);	 //TODO: add a method of generating the level

	void BasicGeneration(FDungeonField* LinkedDugeon);

	void LinkRooms(FDungeonCell RoomA, FDungeonCell RoomB);

	void SetStartingRoom(FVector2D StartPosition);
	//

	void InitGameLevel(FDungeonField* LinkedDungeon);		 // Actual UE level generation on basis of dungeon data

	//static ARoomAsset* GetWorldRoom(FVector2D Coords)   // dilemma, whether to keep pointer in dungeon cell, or make a separate array for them

private:
	FVector2D StartPoint;			// players starting cell

	FDungeonField Dungeon;


	/// for other methods
	/// lookup generation for wang tiles
};
