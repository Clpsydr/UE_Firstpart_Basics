// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomAsset.h"

// Sets default values
ARoomAsset::ARoomAsset()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoomAsset::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

