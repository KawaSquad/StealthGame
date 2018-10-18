// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();

	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompletedMission(APawn * InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn)
	{
		if (SpectatingViewpointClass != nullptr)
		{
			TArray <AActor*> OutActors;

			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, OutActors);

			if (OutActors.Num() > 0)
			{
				AActor *NewViewActor = OutActors[0];
				for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
				{
					APlayerController* PC = It->Get();
					if (PC)
					{
						PC->SetViewTargetWithBlend(NewViewActor, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spectating class == nullptr"));
		}
	}

	AFPSGameState * GS = GetGameState<AFPSGameState>();
	if(GS)
		GS->MulticastOnMissionCompleted(InstigatorPawn,bMissionSuccess);

	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}
