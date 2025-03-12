// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSportsCarPawn.h"

#include "./../AirRaceGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "FunctionalUIScreenshotTest.h"
#include "AirRace/AI/VehicleAIController.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"
#include "Misc/DateTime.h"

#include "Kismet/GameplayStatics.h"


const FName ABaseSportsCarPawn::EngineAudioRPM("RPM");

void ABaseSportsCarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseSportsCarPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseSportsCarPawn::MoveRight);

	PlayerInputComponent->BindAction("Brake", IE_Pressed, this, &ABaseSportsCarPawn::OnBrakePressed);
	PlayerInputComponent->BindAction("Brake", IE_Released, this, &ABaseSportsCarPawn::OnBrakeReleased);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ABaseSportsCarPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ABaseSportsCarPawn::OnHandbrakeReleased);
	PlayerInputComponent->BindAction("Nitro", IE_Pressed, this, &ABaseSportsCarPawn::OnNitroPressed);
	PlayerInputComponent->BindAction("Nitro", IE_Released, this, &ABaseSportsCarPawn::OnNitroReleased);
}

void ABaseSportsCarPawn::Tick(float Delta)
{
	Super::Tick(Delta);
	float RPMToAudioScale = 2500.0f / GetWheeledVehicleMovementComponent()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM,
	                                        GetWheeledVehicleMovementComponent()->GetEngineRotationSpeed() *
	                                        RPMToAudioScale);
	UpdateSpeedKPH();
	ProcessSpeedPerHundred();
	UpdateHUDStrings();
	SetupHUD();
	CheckCarAlive();
	ToggleIsFlying();
}

void ABaseSportsCarPawn::BeginPlay()
{
	Super::BeginPlay();
	EngineSoundComponent->Play();
}

ABaseSportsCarPawn::ABaseSportsCarPawn()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Vehicles/SportsCar/SKM_SportsCar.SKM_SportsCar'"));
	if (SkeletalMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh.Object);
		GetMesh()->SetSimulatePhysics(true);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBPClass(
		TEXT("/Script/Engine.AnimBlueprint'/Game/Vehicles/SportsCar/SportsCar_AnimBP.SportsCar_AnimBP'"));
	if (AnimBPClass.Succeeded())
	{
		GetMesh()->SetAnimClass(AnimBPClass.Object->GeneratedClass);
	}

	// Setup friction materials
	// static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> SlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/Slippery.Slippery"));
	// SlipperyMaterial = SlipperyMat.Object;

	// static ConstructorHelpers::FObjectFinder<UPhysicalMaterial> NonSlipperyMat(TEXT("/Game/VehicleAdv/PhysicsMaterials/NonSlippery.NonSlippery"));
	// NonSlipperyMaterial = NonSlipperyMat.Object;

	WheeledVehicleMovementComponent = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Wheels %d"), Vehicle->Wheels.Num()));
	// Wheels/Tyres

	static ConstructorHelpers::FObjectFinder<UBlueprint> WheelsFront(
		TEXT(
			"/Script/Engine.Blueprint'/Game/VehicleTemplate/Blueprints/SportsCar/SportsCar_WheelsFront.SportsCar_WheelsFront'"));

	static ConstructorHelpers::FObjectFinder<UBlueprint> WheelsRear(
		TEXT(
			"/Script/Engine.Blueprint'/Game/VehicleTemplate/Blueprints/SportsCar/SportsCar_WheelsRear.SportsCar_WheelsRear'"));
	// // Setup the wheels ???


	// Wheel setup
	WheeledVehicleMovementComponent->bSuspensionEnabled = true;
	WheeledVehicleMovementComponent->bWheelFrictionEnabled = true;
	WheeledVehicleMovementComponent->bLegacyWheelFrictionPosition = true;

	// Wheel setups
	if (WheelsFront.Succeeded() && WheelsRear.Succeeded())
	{
		FChaosWheelSetup WheelFlConfig;
		WheelFlConfig.BoneName = FName("Phys_Wheel_FL");
		WheelFlConfig.WheelClass = WheelsFront.Object->GeneratedClass;
		WheelFlConfig.AdditionalOffset = FVector(0.f, 0.0f, 0.f);
		WheeledVehicleMovementComponent->WheelSetups.Add(WheelFlConfig);

		FChaosWheelSetup WheelFRConfig;
		WheelFRConfig.BoneName = FName("Phys_Wheel_FR");
		WheelFRConfig.WheelClass = WheelsFront.Object->GeneratedClass;
		WheelFRConfig.AdditionalOffset = FVector(0.f, 0.f, 0.f);
		WheeledVehicleMovementComponent->WheelSetups.Add(WheelFRConfig);

		FChaosWheelSetup WheelBLConfig;
		WheelBLConfig.BoneName = FName("Phys_Wheel_BL");
		WheelBLConfig.WheelClass = WheelsRear.Object->GeneratedClass;
		WheelBLConfig.AdditionalOffset = FVector(0.f, 0.f, 0.f);
		WheeledVehicleMovementComponent->WheelSetups.Add(WheelBLConfig);

		FChaosWheelSetup WheelBRConfig;
		WheelBRConfig.BoneName = FName("Phys_Wheel_BR");
		WheelBRConfig.WheelClass = WheelsRear.Object->GeneratedClass;
		WheelBRConfig.AdditionalOffset = FVector(0.f, 0.f, 0.f);
		WheeledVehicleMovementComponent->WheelSetups.Add(WheelBRConfig);
	}
	// Setup wheels
	for (UChaosVehicleWheel* Wheel : WheeledVehicleMovementComponent->Wheels)
	{
		if (Wheel)
		{
			Wheel->CorneringStiffness = 100000.0f; // Very high stiffness
			Wheel->SuspensionDampingRatio = 5000.0f; // High damping to reduce bounce
			// Wheel->SuspensionCompressionDamping = 5000.0f; // Quick response to compression
			// Wheel->SuspensionRelaxationDamping = 5000.0f; // Quick return to position
			Wheel->SuspensionMaxRaise = 1.0f; // Minimal lift
			Wheel->SuspensionMaxDrop = 1.0f; // Minimal drop
			// Wheel->SuspensionLength = 10.0f; // Very short travel range
		}
	}
	// Mechanical Setup
	WheeledVehicleMovementComponent->bMechanicalSimEnabled = true;

	// Engine Setup
	static ConstructorHelpers::FObjectFinder<UCurveFloat> TorqueCurve(
		TEXT(
			"/Script/Engine.CurveFloat'/Game/VehicleTemplate/Blueprints/SportsCar/SportsCar_TorqueCurve.SportsCar_TorqueCurve'"));
	if (TorqueCurve.Succeeded())
	{
		WheeledVehicleMovementComponent->EngineSetup.TorqueCurve.ExternalCurve = TorqueCurve.Object;
	}
	WheeledVehicleMovementComponent->EngineSetup.MaxTorque = MaxTorque;
	WheeledVehicleMovementComponent->EngineSetup.MaxRPM = 7000.0f;
	WheeledVehicleMovementComponent->EngineSetup.EngineIdleRPM = 900.0f;
	WheeledVehicleMovementComponent->EngineSetup.EngineBrakeEffect = 0.2f;
	WheeledVehicleMovementComponent->EngineSetup.EngineRevUpMOI = 5.0f;
	WheeledVehicleMovementComponent->EngineSetup.EngineRevDownRate = 600.0f;

	// Differential Setup
	WheeledVehicleMovementComponent->DifferentialSetup.DifferentialType = EVehicleDifferential::RearWheelDrive;
	WheeledVehicleMovementComponent->DifferentialSetup.FrontRearSplit = 0.5f;

	// Transmission Setup
	WheeledVehicleMovementComponent->TransmissionSetup.bUseAutomaticGears = true;
	WheeledVehicleMovementComponent->TransmissionSetup.bUseAutoReverse = true;
	WheeledVehicleMovementComponent->TransmissionSetup.FinalRatio = 2.81f;
	WheeledVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Empty();
	WheeledVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Add(4.25f);
	WheeledVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Add(2.52f);
	WheeledVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Add(1.66f);
	WheeledVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Add(1.22f);
	WheeledVehicleMovementComponent->TransmissionSetup.ForwardGearRatios.Add(1.0f);
	WheeledVehicleMovementComponent->TransmissionSetup.ReverseGearRatios.Empty();
	WheeledVehicleMovementComponent->TransmissionSetup.ReverseGearRatios.Add(4.04f);
	WheeledVehicleMovementComponent->TransmissionSetup.ChangeUpRPM = 6000.0f;
	WheeledVehicleMovementComponent->TransmissionSetup.ChangeDownRPM = 2000.0f;
	WheeledVehicleMovementComponent->TransmissionSetup.GearChangeTime = 0.2f;
	WheeledVehicleMovementComponent->TransmissionSetup.TransmissionEfficiency = 0.9f;

	// Vehicle Setup
	WheeledVehicleMovementComponent->ChassisHeight = 144.0f;
	WheeledVehicleMovementComponent->Mass = 1500.0f;
	// WheeledVehicleMovementComponent->Thrusters = 

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	SpringArm->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 150.0f);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 550.0f;  // Default distance
	// SpringArm->CameraLagMaxDistance = 650.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->CameraLagSpeed = 5.0f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 5.0f;
	SpringArm->bUsePawnControlRotation = false;  // Free movement
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	// SpringArm->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	// SpringArm->SocketOffset = FVector(0.0f, 0.0f, 150.0f);
	// SpringArm->SetupAttachment(RootComponent);
	// SpringArm->TargetArmLength = 650.0f;
	// SpringArm->bEnableCameraLag = false;
	// SpringArm->bEnableCameraRotationLag = false;
	// SpringArm->bInheritPitch = true;
	// SpringArm->bInheritYaw = true;
	// SpringArm->bInheritRoll = true;


	// Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	// Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	// Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	// Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	// Camera->bUsePawnControlRotation = false;
	// Camera->ProjectionMode = ECameraProjectionMode::Perspective;
	// Camera->FieldOfView = 90.f;


	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(
		TEXT("/Script/Engine.SoundCue'/Game/Vehicles/SportsCar/Sound/Engine_Loop_Cue.Engine_Loop_Cue'"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(GetMesh());

	SpeedPerHundred = FText::FromString("NO INFO");

	AIControllerClass = AVehicleAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

AController* ABaseSportsCarPawn::GetController() const
{
	return Cast<AController>(AVehicleAIController::StaticClass());
}

AController* ABaseSportsCarPawn::GetDefaultController() const
{
	return Cast<AController>(AVehicleAIController::StaticClass());
}

UChaosWheeledVehicleMovementComponent* ABaseSportsCarPawn::GetWheeledVehicleMovementComponent()
{
	return WheeledVehicleMovementComponent;
}

void ABaseSportsCarPawn::MoveForward(float Val)
{
	if (bIsFlying)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Start flying")));
		// Get car's mesh and physics body
		UPrimitiveComponent* MeshComp = Cast<UPrimitiveComponent>(GetRootComponent());
		if (!MeshComp) return;
		MeshComp->AddTorqueInRadians(FVector(0.0f, Val * 0.5f, 0.0f), NAME_None, true);
	}
	else
	{
		GetVehicleMovement()->SetThrottleInput(Val);
	}
}

void ABaseSportsCarPawn::MoveRight(float Val)
{
	if (bIsFlying)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Start flying")));
		// Get car's mesh and physics body
		UPrimitiveComponent* MeshComp = Cast<UPrimitiveComponent>(GetRootComponent());
		if (!MeshComp) return;
		
		MeshComp->AddTorqueInRadians(FVector(-Val * 0.5f, 0.0f * Val, 0.0f), NAME_None, true);
	}
	else
	{
		GetVehicleMovement()->SetSteeringInput(Val);
	}
}


void ABaseSportsCarPawn::OnHandbrakePressed()
{
	GetVehicleMovement()->SetHandbrakeInput(true);
}

void ABaseSportsCarPawn::OnHandbrakeReleased()
{
	GetVehicleMovement()->SetHandbrakeInput(false);
}

void ABaseSportsCarPawn::OnBrakePressed()
{
	GetVehicleMovement()->SetBrakeInput(1.0f);
}

void ABaseSportsCarPawn::OnBrakeReleased()
{
	GetVehicleMovement()->SetBrakeInput(0.0f);
}

void ABaseSportsCarPawn::SetupHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr))
	{
		InCarSpeed->SetText(SpeedDisplayString);
	}
}

inline void ABaseSportsCarPawn::CheckCarAlive()
{
	if (this->GetActorLocation().Z < 0)
	{
		AAirRaceGameMode* GameMode = Cast<AAirRaceGameMode>(UGameplayStatics::GetGameMode(this));
		if (GameMode)
		{
			GameMode->GameOver();
			GameMode->RestartGame();
		}
	}
}


void ABaseSportsCarPawn::OnNitroPressed()
{
	GetWheeledVehicleMovementComponent()->SetMaxEngineTorque(MaxTorqueNitro);
}

void ABaseSportsCarPawn::OnNitroReleased()
{
	GetWheeledVehicleMovementComponent()->SetMaxEngineTorque(MaxTorque);
}

bool ABaseSportsCarPawn::IsFlying()
{
	if (WheeledVehicleMovementComponent)
	{
		int32 NumWheels = WheeledVehicleMovementComponent->Wheels.Num();
		for (int32 i = 0; i < NumWheels; i++)
		{
			if (WheeledVehicleMovementComponent->Wheels[i]->IsInAir() == false)
			{
				return false; // At least one wheel is on the ground
			}
		}
		return true; // All wheels are in the air
	}
	return false; // No movement component found
}

void ABaseSportsCarPawn::UpdateSpeedKPH()
{
	SpeedKPH = FMath::Abs(GetWheeledVehicleMovementComponent()->GetForwardSpeed()) * 0.036f;
}

void ABaseSportsCarPawn::ProcessSpeedPerHundred()
{
	UpdateSpeedPerHundredStart();
	UpdateSpeedPerHundredFinish();
	if (bSpeedPerHundredStarted && bSpeedPerHundredFinished)
	{
		ShowSpeedPerHundred();
		ResetSpeedPerHundred();
	}
}

void ABaseSportsCarPawn::UpdateSpeedPerHundredStart()
{
	if (!bSpeedPerHundredStarted && !bSpeedPerHundredFinished && SpeedKPH > 0.1f && SpeedKPH < 100.0f)
	{
		bSpeedPerHundredStarted = true;
		SpeedPerHundredStartTime = FDateTime().Now();
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		//                                  FString::Printf(TEXT("UpdateSpeedPerHundredStart %f"), SpeedKPH));
	}
}

void ABaseSportsCarPawn::UpdateSpeedPerHundredFinish()
{
	if (!bSpeedPerHundredFinished && bSpeedPerHundredStarted && SpeedKPH > 100.0f)
	{
		bSpeedPerHundredFinished = true;
		SpeedPerHundredFinishTime = FDateTime().Now();
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		//                                  FString::Printf(TEXT("UpdateSpeedPerHundredFinish %f"), SpeedKPH));
	}
}


void ABaseSportsCarPawn::ResetSpeedPerHundred()
{
	bSpeedPerHundredStarted = false;
	bSpeedPerHundredFinished = false;
}

void ABaseSportsCarPawn::ToggleIsFlying()
{
	bIsFlying = IsFlying();
	// if (!bIsFlying)
	// {
	if (IsFlying())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Start flying")));
		// // Get car's mesh and physics body
		// UPrimitiveComponent* MeshComp = Cast<UPrimitiveComponent>(GetRootComponent());
		// if (!MeshComp) return;
		//
		// // Calculate roll torque
		// FVector Torque = FVector(0.0f, 0.0f, 0.0f);
		// float RollStrength = 500000.0f; // Adjust this value for more/less rolling
		// Torque.X = 1.0f * RollStrength;  // Roll around X-axis
		// // Apply torque
		// MeshComp->AddTorqueInRadians(Torque, NAME_None, true);

		// GetVehicleMovement()->SetYawInput(1.0f);
		// GetVehicleMovement()->SetRollInput(1.0f);
		// GetVehicleMovement()->SetPitchInput(1.0f);
	}
	// }	
}


#define LOCTEXT_NAMESPACE "SportsCarPawn"

void ABaseSportsCarPawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetWheeledVehicleMovementComponent()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);
	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));
}

void ABaseSportsCarPawn::ShowSpeedPerHundred()
{
	FTimespan Timespan = SpeedPerHundredFinishTime - SpeedPerHundredStartTime;
	SpeedPerHundred = FText::Format(
		LOCTEXT("SpeedPerHundredFormat", "Until 100KPH is {0}"), FText::FromString(Timespan.ToString()));
}

#undef LOCTEXT_NAMESPACE
