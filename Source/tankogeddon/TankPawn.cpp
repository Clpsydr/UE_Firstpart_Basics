// Fill out your copyright notice in the Description page of Project Settings.
#include "TankPawn.h"
#include "Cannon.h"
#include "AmmoBox.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/ArrowComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "BulletPoolSubsystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "PatrolAIController.h"
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

	MainCannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Main Weapon spawn point"));
	MainCannonSpawnPoint->SetupAttachment(TurretMesh);

	SubCannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Sub Weapon spawn point"));
	SubCannonSpawnPoint->SetupAttachment(TurretMesh);

	DamageEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Damage Effect"));
	DamageEffect->SetupAttachment(BodyMesh);
	
	DestructionEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Death Effect"));
	DestructionEffect->SetupAttachment(BodyMesh);

	DeathSoundEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("Death Sound"));
	DeathSoundEffect->SetupAttachment(BodyMesh);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(BodyMesh);

	TankHP = CreateDefaultSubobject<UHPcomponent>(TEXT("Health component"));
	TankHP->OnHealthChanged.AddDynamic(this, &ATankPawn::OnHealthChanged);
	TankHP->OnDie.AddDynamic(this, &ATankPawn::OnDie);
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Instigator = this;
	Params.Owner = this;

	Cannon = GetWorld()->SpawnActor<ACannon>(MainCannonClass, Params);
	Cannon->AttachToComponent(MainCannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	SubWeapon = GetWorld()->SpawnActor<ACannon>(SubWeaponClass, Params);
	SubWeapon->AttachToComponent(SubCannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);

	ActiveWeapon = Cannon;
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float FPSIndependentMotion = exp2(-TankMotionSmoothness * DeltaTime);

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
	
	TurretMesh->SetWorldRotation(FMath::RInterpTo(CurrentTurretRotation, TargetTurretRotation, DeltaTime, TurretMotionSmoothness));
	
	/*TargetTurretRotation.Yaw = FMath::FInterpConstantTo(CurrentTurretRotation.Yaw, TargetTurretRotation.Yaw, DeltaTime, TurretMotionSmoothness);
	//TurretMesh->SetWorldRotation(CurrentTurretRotation);	
	float RotationTurret = CurrentTurretRotation.Yaw + TargetTurretRotation.Yaw * RotationTurret * DeltaTime;
	TurretMesh->SetWorldRotation(FRotator(0.f, RotationTurret, 0.f));
	
	//TargetTurretRotation.Yaw = FMath::FInterpConstantTo(CurrentTurretRotation.Yaw, TargetTurretRotation.Yaw, DeltaTime, TurretMotionSmoothness);
	//TurretMesh->SetWorldRotation(FMath::InterpExpoOut(CurrentTurretRotation, TargetTurretRotation, FPSIndependentMotion));*/
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
	if (ActiveWeapon)
	{
		ActiveWeapon->Fire();
	}
}

void ATankPawn::AltFire()
{
	if (ActiveWeapon)
	{
		ActiveWeapon->AltFire();
	}
}

void ATankPawn::WeapChange()
{
	if (ActiveWeapon == Cannon)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("swapping from cannon to sub"));
		ActiveWeapon = SubWeapon;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("swapping from sub to cannon"));
		ActiveWeapon = Cannon;
	}
}

void ATankPawn::RefillAmmo(ECannonType cannontype, int amount)
{
	if (Cannon->GetType() == cannontype)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("refilling main cannon"));
		Cannon->Refill(amount);
	}
	if (SubWeapon->GetType() == cannontype)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("refilling sub cannon"));
		SubWeapon->Refill(amount);
	}
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::TakeDamage(const FDamageData& DamageData)
{
	DamageEffect->ActivateSystem();
	TankHP->TakeDamage(DamageData);
}

void ATankPawn::OnHealthChanged_Implementation(float Damage)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("Tank HP left " + FString::SanitizeFloat(TankHP->GetHPRatio() * 100) + "%"));
}

// instead of leaving sound outside of the pawn (which is probably the right way), I make the tank intangible to let it play the effects
void ATankPawn::OnDie_Implementation()
{
	DestructionEffect->ActivateSystem();
	DeathSoundEffect->Play(); 

	UBulletPoolSubsystem* BulletPool = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
	FTransform SpawnTransform(GetActorRotation(),GetActorLocation(), FVector::OneVector);
	AAmmoBox* AmmoBox = Cast<AAmmoBox>(BulletPool->RetrieveActor(ItemDrop, SpawnTransform));

	PrimaryActorTick.SetTickFunctionEnable(false);
	BodyMesh->SetHiddenInGame(true);
	TurretMesh->SetHiddenInGame(true);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	Cast<APatrolAIController>(GetController())->TurnOff(); //Bad stuff, code will die together with player pawn
	GetController()->UnPossess();
	SubWeapon->Destroy(); 
	Cannon->Destroy();  // Throws exception, probably because something still attempts to call for the cannon
	
	SetLifeSpan(3.f);
}