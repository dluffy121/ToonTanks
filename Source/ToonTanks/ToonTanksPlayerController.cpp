// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksPlayerController.h"
#include <GameFramework/Pawn.h>

void AToonTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    auto pawn = GetPawn();

    if (!pawn) return;

    if (bPlayerEnabled)
        pawn->EnableInput(this);
    else
        pawn->DisableInput(this);

    bShowMouseCursor = bPlayerEnabled;
}
