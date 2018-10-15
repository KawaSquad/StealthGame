// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "FPSCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	//BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	//BoxComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);
	//MAYBE HERE FOR BOX

	BoxComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter * MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn)
	{
		MyPawn->LaunchCharacter(ForceVector,true,true);
		if (JumpSound)	
			UGameplayStatics::PlaySound2D(this, JumpSound);
	}

	if (OtherComp && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulse(ForceVector, NAME_None, true);
		if (JumpSound)
			UGameplayStatics::PlaySound2D(this, JumpSound);
	}
}

// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

