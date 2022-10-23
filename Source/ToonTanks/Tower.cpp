// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "ToonTanksGameMode.h"
#include <Kismet/GameplayStatics.h>
#include <TimerManager.h>

// Sets default values
ATower::ATower()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::HandleDestruction()
{
    Super::HandleDestruction();

    Destroy();
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (IsInRange())
        RotateTurret(Tank->GetActorLocation(), FVector(1.f, 1.f, 0.f));
}

void ATower::CheckFireCondition()
{
    if (IsInRange() && Tank && !Tank->IsDead())
        Fire();
}

bool ATower::IsInRange() const
{
    return Tank && FVector::Distance(Tank->GetActorLocation(), GetActorLocation()) <= ThreatDetectionRadius;
}