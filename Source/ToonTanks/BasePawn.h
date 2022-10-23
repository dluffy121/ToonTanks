// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"

UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		class UParticleSystem* DeathParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		class USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UCameraShakeBase> DeathCamerShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Movement", meta = (AllowPrivateAccess = "true"))
		float TurretTurnRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret Movement", meta = (AllowPrivateAccess = "true"))
		FVector TurretOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret Movement", meta = (AllowPrivateAccess = "true"))
		FVector TurretMinRotation = -FVector(180.f, 180.f, 180.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret Movement", meta = (AllowPrivateAccess = "true"))
		FVector TurretMaxRotation = FVector(180.f, 180.f, 180.f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
		bool Dead = false;

public:
	// Sets default values for this pawn's properties
	ABasePawn();

	void HandleDestruction();
	inline bool IsDead() const { return Dead; }

protected:
	void RotateTurret(FVector lookAtTarget, FVector rotateAxes);
	void Fire();
};
