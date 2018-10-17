// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"


// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSee);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHear);
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
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

	DrawDebugSphere(GetWorld(), Pawn->GetActorLocation(), 32.0f, 12, FColor::Yellow, false, 10.0f);
}

void AFPSAIGuard::OnNoiseHear(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();
	FRotator NewLook = FRotationMatrix::MakeFromX(Direction).Rotator();

	NewLook.Pitch = 0.0f;
	NewLook.Roll = 0.0f;

	SetActorRotation(NewLook, ETeleportType::None);

	GetWorldTimerManager().ClearTimer(Timerhandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(Timerhandle_ResetOrientation,this,&AFPSAIGuard::ResetOrientation,3.0f);

	DrawDebugSphere(GetWorld(), Location, 32.0f*Volume, 12, FColor::Red, false, 10.0f);
}

void AFPSAIGuard::ResetOrientation()
{
	SetActorRotation(OriginalRotation);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

