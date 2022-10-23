// Fill out your copyright notice in the Description page of Project Settings.

#include "BasePawn.h"
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Projectile.h"

// Sets default values
ABasePawn::ABasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	CapsuleComp->SetupAttachment(BaseMesh);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::HandleDestruction()
{
	if (DeathParticles)
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	if (DeathSound)
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());

	Dead = true;
}

void ABasePawn::RotateTurret(FVector lookAtTarget, FVector rotateAxes)
{
	FVector turretDir = lookAtTarget - (TurretMesh->GetComponentLocation() - TurretOffset);

	FRotator turretRotation = FRotator(
		turretDir.Rotation().Pitch * rotateAxes.X,
		turretDir.Rotation().Yaw * rotateAxes.Y,
		turretDir.Rotation().Roll * rotateAxes.Z);

	turretRotation = FMath::RInterpTo(
		TurretMesh->GetComponentRotation(),
		turretRotation,
		UGameplayStatics::GetWorldDeltaSeconds(this),
		TurretTurnRate);

	turretRotation.Pitch = FMath::Clamp(turretRotation.Pitch, TurretMinRotation.X, TurretMaxRotation.X);
	turretRotation.Yaw = FMath::Clamp(turretRotation.Yaw, TurretMinRotation.Y, TurretMaxRotation.Y);
	turretRotation.Roll = FMath::Clamp(turretRotation.Roll, TurretMinRotation.Z, TurretMaxRotation.Z);

	TurretMesh->SetWorldRotation(turretRotation);
}

void ABasePawn::Fire()
{
	auto projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	projectile->SetOwner(this);
}