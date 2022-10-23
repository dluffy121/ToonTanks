// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"
#include <Kismet/GameplayStatics.h>

void AToonTanksGameMode::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    TankPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    StartGame();

    if (TankPlayerController)
    {
        TankPlayerController->SetPlayerEnabledState(false);

        FTimerHandle timerHandle;
        FTimerDelegate timerDel = FTimerDelegate::CreateUObject(
            TankPlayerController,
            &AToonTanksPlayerController::SetPlayerEnabledState,
            true
        );

        GetWorldTimerManager().SetTimer(
            timerHandle,
            timerDel,
            StartDelay,
            false
        );
    }
}

void AToonTanksGameMode::ActorDied(AActor* deadActor)
{
    if (Tank && deadActor == Tank)
    {
        Tank->HandleDestruction();
        if (TankPlayerController)
            TankPlayerController->SetPlayerEnabledState(false);

        GameOver(false);
    }
    else if (ATower* destroyedTower = Cast<ATower>(deadActor))
    {
        destroyedTower->HandleDestruction();
        --TargetTowers;
        if (TargetTowers == 0)
            GameOver(true);
    }
}

int32 AToonTanksGameMode::GetTowerCount() const
{
    TArray<AActor*> towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), towers);
    return towers.Num();
}