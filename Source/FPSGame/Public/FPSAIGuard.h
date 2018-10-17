// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle, Suspicious, Alarted,
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere,Category="Component")
	UPawnSensingComponent * PawnSensingComp;

	UFUNCTION()
		void OnPawnSee(APawn* Pawn);

	UFUNCTION()
		void OnNoiseHear(APawn* NoiseInstigator, const FVector& Location, float Volume);

	UFUNCTION()
		void ResetOrientation();

	FRotator OriginalRotation;

	FTimerHandle Timerhandle_ResetOrientation;

	EAIState EGuardState;

	UFUNCTION()
		void SetGuardState(EAIState NewState);
	UFUNCTION()
		void GotoTarget();
	UFUNCTION()
		void GotoNextTarget();


	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void OnStateChange(EAIState NewState);


	UPROPERTY(EditInstanceOnly, Category = "AI")
		bool isPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "isPatrol"))
		TArray<AActor*> targets;

	UPROPERTY(VisibleAnywhere, Category = "AI", meta = (EditCondition = "isPatrol"))
		int indexTarget;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;	
	
};
