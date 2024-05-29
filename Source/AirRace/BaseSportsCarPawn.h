// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "BaseSportsCarPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UChaosWheeledVehicleMovementComponent;
class UAudioComponent;
class UTextRenderComponent;
/**
 * 
 */
UCLASS()
class AIRRACE_API ABaseSportsCarPawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()
	
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float Delta) override;
	virtual void BeginPlay() override;
public:
    ABaseSportsCarPawn();
public:
    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;
	
    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

     UPROPERTY(Category = Vehicle, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	 UChaosWheeledVehicleMovementComponent* WheeledVehicleMovementComponent;
	/** Audio component for the engine sound */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* EngineSoundComponent;
	
public:
	FORCEINLINE UChaosWheeledVehicleMovementComponent* GetWheeledVehicleMovementComponent();
	/** Handle pressing forwards */
	void MoveForward(float Val);
	/** Handle pressing right */
	void MoveRight(float Val);
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();
	/** Setup the strings used on the hud */
	void SetupHUD();
	void UpdateHUDStrings();

	static const FName EngineAudioRPM;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	FText SpeedDisplayString;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* InCarSpeed;
};
