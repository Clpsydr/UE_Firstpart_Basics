// Fill out your copyright notice in the Description page of Project Settings.
#include "TankPawn.h"
#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "tankogeddon.h"

// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	CannonSpawnPoint->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	SetupCannon();
	
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
// Tried two ways here, for tank motion there is a manual calculation that I looked up online
// For turret however theres an issue, with crossing zero degree point, lerping would happen to occur
// over larger distance due to degree leaping from 0 to 360
	Super::Tick(DeltaTime);
	
	float FPSIndependentMotion = exp2(-TankMotionSmoothness * DeltaTime);
	//UE_LOG(LogTankgeddon, Verbose, TEXT("FPSMotion: %f"), currentfps);

	CurrentForwardAxisValue = FMath::Lerp(CurrentForwardAxisValue, TargetForwardAxisValue, FPSIndependentMotion);

	FVector moveVector = GetActorForwardVector() * CurrentForwardAxisValue;
	FVector movePosition = GetActorLocation() + MoveSpeed * moveVector * DeltaTime;
	SetActorLocation(movePosition, true);

	CurrentRotationValue = FMath::Lerp(CurrentRotationValue, TargetRotationValue, FPSIndependentMotion);
	float Rotation = GetActorRotation().Yaw + CurrentRotationValue * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0.f, Rotation, 0.f));

		//rotation angle from one vector to another
	FRotator TargetTurretRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetAngle);
	FRotator CurrentTurretRotation = TurretMesh->GetComponentRotation();
	TargetTurretRotation.Roll = CurrentTurretRotation.Roll;
	TargetTurretRotation.Pitch = CurrentTurretRotation.Pitch;
	
	FPSIndependentMotion = exp2(-TurretMotionSmoothness * DeltaTime);
	TargetTurretRotation.Yaw = FMath::FInterpConstantTo(CurrentTurretRotation.Yaw, TargetTurretRotation.Yaw, DeltaTime, TurretMotionSmoothness);
	TurretMesh->SetWorldRotation(TargetTurretRotation);
	//TurretMesh->SetWorldRotation(FMath::InterpExpoOut(CurrentTurretRotation, TargetTurretRotation, FPSIndependentMotion));
	//TurretMesh->SetWorldRotation(FMath::Lerp(CurrentTurretRotation, TargetTurretRotation, FPSIndependentMotion));
}

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankPawn::RotateRight(float Torque)
{
	TargetRotationValue = Torque;
}

void ATankPawn::SetTurretTargetAngle(const FVector& TargetDirection)
{
	TurretTargetAngle = TargetDirection;
}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::AltFire()
{
	if (Cannon)
	{
		Cannon->AltFire();
	}
}

void ATankPawn::SetupCannon()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(DefaultCannonClass, Params);
	Cannon->AttachToComponent(CannonSpawnPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ATankPawn::WeapChange()
{
	if (Cannon)
	{
		Cannon->CycleWeapons();
	}
}