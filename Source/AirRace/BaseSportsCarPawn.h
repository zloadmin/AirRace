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
	void MoveForwardTouch(float Val);
	/** Handle pressing right */
	void MoveRight(float Val);
	// /** RotationRate */
	// void RotationRate(float Val);
	// /** Tilt */
	void Tilt(float Val);
	// /** Gravity */
	// void Gravity(float Val);
	// /** Acceleration */
	// void Acceleration(float Val);
	/** RotationRate */
	void RotationRate(FVector Val);
	/** Tilt */
	void Tilt(FVector Val);
	void GamePad(float Val);
	/** Gravity */
	void Gravity(FVector Val);
	/** Acceleration */
	void Acceleration(FVector Val);
	/** Handle handbrake pressed */
	void OnHandbrakePressed();
	/** Handle handbrake released */
	void OnHandbrakeReleased();
	/** Setup the strings used on the hud */
	void SetupHUD();
	/** Update Hub string data */
	void UpdateHUDStrings();
	/** Handle nitro pressed */
	void OnNitroPressed();
	/** Handle nitro released */
	void OnNitroReleased();

	/** Check is car not fail down */
	void CheckCarAlive();
	/** Android controller */
	void AndroidControl();

	static const FName EngineAudioRPM;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
	FText SpeedDisplayString;
	FText SpeedPerHundred;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTextRenderComponent* InCarSpeed;
private:
	void UpdateSpeedKPH();

	void ProcessSpeedPerHundred();
	void UpdateSpeedPerHundredStart();
	void UpdateSpeedPerHundredFinish();
	void ShowSpeedPerHundred();
	void ResetSpeedPerHundred();
private :
	bool bIsNitroEnable = false;
	float MaxTorque = 750.0f; 
	float MaxTorqueNitro = 3000.0f;
	float SpeedKPH = 0.0f;
	
	bool bSpeedPerHundredStarted = false;
	bool bSpeedPerHundredFinished = false;
	FDateTime SpeedPerHundredStartTime;
	FDateTime SpeedPerHundredFinishTime;
};








