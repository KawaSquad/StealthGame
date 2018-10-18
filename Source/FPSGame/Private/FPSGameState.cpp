// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSGameState.h"
#include "FPSPlayerController.h"


void AFPSGameState::MulticastOnMissionCompleted_Implementation(APawn * InstigatorPawn, bool bMissionsSuccess)
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		AFPSPlayerController * PlayerCtrl = Cast<AFPSPlayerController>(It->Get());
		if (PlayerCtrl && PlayerCtrl->IsLocalController())
		{
			PlayerCtrl->OnMissionCompleted(InstigatorPawn,bMissionsSuccess);
			APawn * Pawn = PlayerCtrl->GetPawn();
			if (Pawn)
				Pawn->DisableInput(PlayerCtrl);
		}
	}
}
