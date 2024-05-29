// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSportsCarPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"

const FName ABaseSportsCarPawn::EngineAudioRPM("RPM");

void ABaseSportsCarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseSportsCarPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseSportsCarPawn::MoveRight);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &ABaseSportsCarPawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &ABaseSportsCarPawn::OnHandbrakeReleased);
}

void ABaseSportsCarPawn::Tick(float Delta)
{
	Super::Tick(Delta);
	float RPMToAudioScale = 2500.0f / GetWheeledVehicleMovementComponent()->GetEngineMaxRotationSpeed();
	EngineSoundComponent->SetFloatParameter(EngineAudioRPM, GetWheeledVehicleMovementComponent()->GetEngineRotationSpeed()*RPMToAudioScale);
	UpdateHUDStrings();
	SetupHUD();
	
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
		TEXT("/Script/Engine.Blueprint'/Game/VehicleTemplate/Blueprints/SportsCar/SportsCar_WheelsFront.SportsCar_WheelsFront'"));

	static ConstructorHelpers::FObjectFinder<UBlueprint> WheelsRear(
		TEXT("/Script/Engine.Blueprint'/Game/VehicleTemplate/Blueprints/SportsCar/SportsCar_WheelsRear.SportsCar_WheelsRear'"));
	// // Setup the wheels ???

	
	// Wheel setup
	WheeledVehicleMovementComponent->bSuspensionEnabled = true;
	WheeledVehicleMovementComponent->bWheelFrictionEnabled = true;
	WheeledVehicleMovementComponent->bLegacyWheelFrictionPosition = true;
	
	// Wheel setups
	if(WheelsFront.Succeeded() && WheelsRear.Succeeded())
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
	// Mechanical Setup
	WheeledVehicleMovementComponent->bMechanicalSimEnabled = true;
	
	// Engine Setup
	static ConstructorHelpers::FObjectFinder<UCurveFloat> TorqueCurve(
		TEXT("/Script/Engine.CurveFloat'/Game/VehicleTemplate/Blueprints/SportsCar/SportsCar_TorqueCurve.SportsCar_TorqueCurve'"));
	if(TorqueCurve.Succeeded())
	{
		WheeledVehicleMovementComponent->EngineSetup.TorqueCurve.ExternalCurve = TorqueCurve.Object;
	}
	WheeledVehicleMovementComponent->EngineSetup.MaxTorque = 750.0f;
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

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 75.0f));
	SpringArm->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 150.0f);  
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 650.0f;
	SpringArm->bEnableCameraLag = false;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = true;


	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->ProjectionMode = ECameraProjectionMode::Perspective;
	Camera->FieldOfView = 90.f;

	
	static ConstructorHelpers::FObjectFinder<USoundCue> SoundCue(TEXT("/Script/Engine.SoundCue'/Game/Vehicles/SportsCar/Sound/Engine_Loop_Cue.Engine_Loop_Cue'"));
	EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
	EngineSoundComponent->SetSound(SoundCue.Object);
	EngineSoundComponent->SetupAttachment(GetMesh());
	
	
	
}

UChaosWheeledVehicleMovementComponent* ABaseSportsCarPawn::GetWheeledVehicleMovementComponent()
{
	return WheeledVehicleMovementComponent;
}

void ABaseSportsCarPawn::MoveForward(float Val)
{
	GetVehicleMovement()->SetThrottleInput(Val);
}

void ABaseSportsCarPawn::MoveRight(float Val)
{
	GetVehicleMovement()->SetSteeringInput(Val);
}

void ABaseSportsCarPawn::OnHandbrakePressed()
{
	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("ABaseSportsCarPawn::OnHandbrakePressed")));
	GetVehicleMovement()->SetHandbrakeInput(true);
}

void ABaseSportsCarPawn::OnHandbrakeReleased()
{
	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("ABaseSportsCarPawn::OnHandbrakeReleased")));
	GetVehicleMovement()->SetHandbrakeInput(false);
}

void ABaseSportsCarPawn::SetupHUD()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if ((PlayerController != nullptr) && (InCarSpeed != nullptr))
	{
		InCarSpeed->SetText(SpeedDisplayString);	
	}
	
}
#define LOCTEXT_NAMESPACE "SportsCarPawn"

void ABaseSportsCarPawn::UpdateHUDStrings()
{
	float KPH = FMath::Abs(GetWheeledVehicleMovementComponent()->GetForwardSpeed()) * 0.036f;
	int32 KPH_int = FMath::FloorToInt(KPH);
	// Using FText because this is display text that should be localizable
	SpeedDisplayString = FText::Format(LOCTEXT("SpeedFormat", "{0} km/h"), FText::AsNumber(KPH_int));

}
#undef LOCTEXT_NAMESPACE
