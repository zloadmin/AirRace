// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "BaseSportsCarPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UChaosWheeledVehicleMovementComponent;

/**
 * 
 */
UCLASS()
class AIRRACE_API ABaseSportsCarPawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
public:
    ABaseSportsCarPawn();
public:
    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	
    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

     UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	 UChaosWheeledVehicleMovementComponent* WheeledVehicleMovementComponent;
public:
	/** Handle pressing forwards */
	void MoveForward(float Val);
	/** Handle pressing right */
	void MoveRight(float Val);
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();
};
