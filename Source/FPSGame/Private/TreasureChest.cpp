// Fill out your copyright notice in the Description page of Project Settings.

#include "TreasureChest.h"
#include "FPSCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"


// Sets default values
ATreasureChest::ATreasureChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATreasureChest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATreasureChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATreasureChest::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	AFPSCharacter * MyCharacter = Cast<AFPSCharacter>(OtherActor);

	if (MyCharacter)
	{

		//UAnimInstance* AnimInstance = SkeletalMeshComp->GetAnimInstance();
		//AnimInstance-> cast<BPA_Chest>(AnimInstance);

		//Destroy();
	}
}
