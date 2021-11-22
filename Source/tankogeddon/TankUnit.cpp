// Fill out your copyright notice in the Description page of Project Settings.
#include "TankUnit.h"
#include "Cannon.h"
#include "AmmoBox.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/ArrowComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "BulletPoolSubsystem.h"
#include "PatrolAIController.h"
#include "TankChunks.h"
#include "tankogeddon.h"

ATankUnit::ATankUnit()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring arm"));
	SpringArm->SetupAttachment(BodyMesh);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	SubCannonSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Sub Weapon spawn point"));
	SubCannonSpawnPoint->SetupAttachment(TurretMesh);

	HP->OnHealthChanged.AddDynamic(this, &ATankUnit::OnHealthChanged);
	HP->OnDie.AddDynamic(this, &ATankUnit::OnDie);
}

void ATankUnit::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters Params;
	Params.Owner = this;
	SubWeapon = GetWorld()->SpawnActor<ACannon>(SubWeaponClass, Params);
	SubWeapon->AttachToComponent(SubCannonSpawnPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);

	ActiveWeapon = Cannon;
}

void ATankUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentForwardAxisValue = FMath::InterpEaseIn(CurrentForwardAxisValue, TargetForwardAxisValue, DeltaTime, TankMotionSmoothness);

	FVector moveVector = GetActorForwardVector() * CurrentForwardAxisValue;
	FVector movePosition = GetActorLocation() + MoveSpeed * moveVector * DeltaTime;
	SetActorLocation(movePosition, true);

	CurrentRotationValue = FMath::Lerp(CurrentRotationValue, TargetRotationValue, TankMotionSmoothness);
	float Rotation = GetActorRotation().Yaw + CurrentRotationValue * RotationSpeed * DeltaTime;
	SetActorRotation(FRotator(0.f, Rotation, 0.f));

	FRotator TargetTurretRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TurretTargetAngle);
	FRotator CurrentTurretRotation = TurretMesh->GetComponentRotation();

	TargetTurretRotation.Roll = CurrentTurretRotation.Roll;
	TargetTurretRotation.Pitch = CurrentTurretRotation.Pitch;

	TurretMesh->SetWorldRotation(FMath::RInterpTo(CurrentTurretRotation, TargetTurretRotation, DeltaTime, TurretMotionSmoothness));
}

void ATankUnit::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = AxisValue;
}

void ATankUnit::RotateRight(float Torque)
{
	TargetRotationValue = Torque;
}

void ATankUnit::SetTurretTargetAngle(const FVector& TargetDirection)
{
	TurretTargetAngle = TargetDirection;
}

void ATankUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATankUnit::AltFire()
{
	if (ActiveWeapon)
	{
		ActiveWeapon->AltFire();
	}
}

void ATankUnit::WeapChange()
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

void ATankUnit::RefillAmmo(ECannonType cannontype, int amount)
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

FVector ATankUnit::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankUnit::OnHealthChanged_Implementation(float Damage)
{	
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Blue, TEXT("Tank HP left " + FString::SanitizeFloat(HP->GetHPRatio() * 100) + "%"));
}

// instead of leaving sound outside of the pawn (which is probably the right way), I make the tank intangible to let it play the effects
void ATankUnit::OnDie_Implementation()
{
	DestructionEffect->ActivateSystem();
	DeathSoundEffect->Play();

	UBulletPoolSubsystem* BulletPool = GetWorld()->GetSubsystem<UBulletPoolSubsystem>();
	FTransform SpawnTransform(GetActorRotation(), GetActorLocation(), FVector::OneVector);
	AAmmoBox* AmmoBox = Cast<AAmmoBox>(BulletPool->RetrieveActor(ItemDrop, SpawnTransform));

	for (int i = 0; i < DestroyEffectNumber; i++)
	{
		ChunkGeneration(FVector((FMath::Rand() % 5) * 0.01 - 0.025, (FMath::Rand() % 5) * 0.01 - 0.025, 1.f), 1.f);
	}

	PrimaryActorTick.SetTickFunctionEnable(false);
	BodyMesh->SetHiddenInGame(true);
	TurretMesh->SetHiddenInGame(true);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Cast<APatrolAIController>(GetController())->TurnOff(); //Bad stuff, code will die together with player pawn
	GetController()->UnPossess();
	SubWeapon->Destroy();
	Cannon->Destroy();  

	SetLifeSpan(2.f);
}

//TODO::transfer instigator impulse there to direct chunks in the appropriate direction with appropriate bullet power
void ATankUnit::ChunkGeneration(FVector HitDirection, float HitPower)
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	ATankChunks* NewChunk = GetWorld()->SpawnActor<ATankChunks>(PostDeathFragments, GetActorLocation()+FVector(0.f,0.f,100.f) , GetActorRotation(), Params);
	UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(NewChunk->GetRootComponent());
	if (Mesh)
	{
		if (Mesh->IsSimulatingPhysics())
		{
			FVector ForceVector = HitDirection;
			Mesh->AddImpulse(ForceVector * HitPower, NAME_None, true);
			Mesh->AddTorque(FVector::RightVector, NAME_None, true);
		}
	}

}