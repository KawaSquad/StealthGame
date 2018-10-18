
// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Engine/TargetPoint.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSee);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHear);

	EGuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	indexTarget = 0;

	if(isPatrol)
		GotoTarget();
}

void AFPSAIGuard::OnPawnSee(APawn* Pawn)
{
	if (Pawn == nullptr)
		return;

	AFPSGameMode * GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompletedMission(Pawn, false);
	}

	SetGuardState(EAIState::Alarted);

	AController * Ctrl = GetController();
	if (Ctrl != nullptr)
	{
		Ctrl->StopMovement();
	}
}

void AFPSAIGuard::OnNoiseHear(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (EGuardState == EAIState::Alarted)
		return;

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLook = FRotationMatrix::MakeFromX(Direction).Rotator();

	NewLook.Pitch = 0.0f;
	NewLook.Roll = 0.0f;

	SetActorRotation(NewLook, ETeleportType::None);

	GetWorldTimerManager().ClearTimer(Timerhandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(Timerhandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, 3.0f);

	SetGuardState(EAIState::Suspicious);

	AController * Ctrl = GetController();
	if (Ctrl != nullptr)
	{
		Ctrl->StopMovement();
	}
}

void AFPSAIGuard::ResetOrientation()
{
	if (EGuardState == EAIState::Alarted)
		return;

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);

	if (isPatrol)
		GotoTarget();
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChange(EGuardState);
}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if(EGuardState == NewState)
		return;

	EGuardState = NewState;
	OnRep_GuardState();

	//OnStateChange(EGuardState);
}

void AFPSAIGuard::GotoNextTarget()
{
	indexTarget++;
	if (indexTarget >= targets.Num())
		indexTarget = 0;

	UNavigationSystem::SimpleMoveToActor(GetController(), targets[indexTarget]);
}
void AFPSAIGuard::GotoTarget()
{
	UNavigationSystem::SimpleMoveToActor(GetController(), targets[indexTarget]);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (targets[indexTarget] != nullptr)
	{
		FVector Delta = GetActorLocation() - targets[indexTarget]->GetActorLocation();
		float DistanceToGoal = Delta.Size();
		//UE_LOG(LogTemp, Warning, TEXT("Delta : %f"),DistanceToGoal);
		if (DistanceToGoal < 100)
		{
			GotoNextTarget();
		}
	}
}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, EGuardState);
}
