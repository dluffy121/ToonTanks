// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComp->SetupAttachment(TurretMesh);
	SpringArmComp->SetRelativeLocation(FVector(0.f, 0.f, 65.f));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	TankPlayerController = Cast<APlayerController>(GetController());
}

void ATank::HandleDestruction()
{
	Super::HandleDestruction();

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TankPlayerController)
	{
		FHitResult hitResult;
		TankPlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			hitResult);

		if (!hitResult.bBlockingHit)
			return;

		if (FVector::Distance(TurretMesh->GetComponentLocation(), hitResult.ImpactPoint) < TurretDeadZone)
			return;

		RotateTurret(hitResult.ImpactPoint, FVector(1.f, 1.f, 0.f));
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Move(float value)
{
	IsReversing = value < 0;
	FVector moveDelta = FVector::ZeroVector;
	moveDelta.X =
		value
		* Speed
		* UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(moveDelta, true);
}

void ATank::Turn(float value)
{
	FRotator turnDelta = FRotator::ZeroRotator;
	turnDelta.Yaw =
		value
		* TurnRate
		* UGameplayStatics::GetWorldDeltaSeconds(this);

	turnDelta.Yaw *= IsReversing ? -1.f : 1.f;
	AddActorLocalRotation(turnDelta, true);
}