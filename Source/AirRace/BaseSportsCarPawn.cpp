// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSportsCarPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Camera/CameraComponent.h"


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

	UChaosWheeledVehicleMovementComponent* Vehicle = Cast<UChaosWheeledVehicleMovementComponent>(GetVehicleMovement());
	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Wheels %d"), Vehicle->Wheels.Num()));
	// Wheels/Tyres
	
	static ConstructorHelpers::FObjectFinder<UBlueprint> WheelsFront(
		TEXT("/Script/Engine.Blueprint'/Game/VehicleTemplate/Blueprints/SportsCar/SportsCar_WheelsFront.SportsCar_WheelsFront'"));

	static ConstructorHelpers::FObjectFinder<UBlueprint> WheelsRear(
		TEXT("/Script/Engine.Blueprint'/Game/VehicleTemplate/Blueprints/SportsCar/SportsCar_WheelsRear.SportsCar_WheelsRear'"));
	// // Setup the wheels ???
	
	
	if(WheelsFront.Succeeded() && WheelsRear.Succeeded())
	{
		FChaosWheelSetup WheelFlConfig;
		WheelFlConfig.BoneName = FName("Phys_Wheel_FL");
		WheelFlConfig.WheelClass = WheelsFront.Object->GeneratedClass;
		WheelFlConfig.AdditionalOffset = FVector(0.f, 0.0f, 0.f);
		Vehicle->WheelSetups.Add(WheelFlConfig);
		
		FChaosWheelSetup WheelFRConfig;
		WheelFRConfig.BoneName = FName("Phys_Wheel_FR");
		WheelFRConfig.WheelClass = WheelsFront.Object->GeneratedClass;
		WheelFRConfig.AdditionalOffset = FVector(0.f, 0.f, 0.f);
		Vehicle->WheelSetups.Add(WheelFRConfig);
		
		FChaosWheelSetup WheelBLConfig;
		WheelBLConfig.BoneName = FName("Phys_Wheel_BL");
		WheelBLConfig.WheelClass = WheelsRear.Object->GeneratedClass;
		WheelBLConfig.AdditionalOffset = FVector(0.f, 0.f, 0.f);
		Vehicle->WheelSetups.Add(WheelBLConfig);

		FChaosWheelSetup WheelBRConfig;
		WheelBRConfig.BoneName = FName("Phys_Wheel_BR");
		WheelBRConfig.WheelClass = WheelsRear.Object->GeneratedClass;
		WheelBRConfig.AdditionalOffset = FVector(0.f, 0.f, 0.f);
		Vehicle->WheelSetups.Add(WheelBRConfig);
	}
	
	// FChaosWheelSetup WheelFlConfig;
	// WheelFlConfig.BoneName = FName("Phys_Wheel_FL");
	// WheelFlConfig.AdditionalOffset = FVector(0.f, 0.0f, 0.f);
	// Vehicle->WheelSetups.Add(WheelFlConfig);
	
	

	
	
	
	
	// Vehicle->SetWheelClass(0, UChaosVehicleWheel::StaticClass());
	// Vehicle->SetWheelClass(1, UChaosVehicleWheel::StaticClass());
	// Vehicle->SetWheelClass(2, UChaosVehicleWheel::StaticClass());
	// Vehicle->SetWheelClass(3, UChaosVehicleWheel::StaticClass());

	Vehicle->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 400.0f);
	Vehicle->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f, 500.0f);
	Vehicle->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f, 400.0f);

	// Adjust the steering ???

	// Transmission	
	// We want 4wd
	Vehicle->DifferentialSetup.DifferentialType = EVehicleDifferential::AllWheelDrive;
	// Drive the front wheels a little more than the rear
	Vehicle->DifferentialSetup.FrontRearSplit = 0.65;

	Vehicle->TransmissionSetup.bUseAutomaticGears = true;
	Vehicle->TransmissionSetup.GearChangeTime = 0.15f;
	// Vehicle->TransmissionSetup.GearAutoBoxLatency = 1.0f; ???
	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle->UpdatedComponent);

	

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

	// GetVehicleMovement()->Set = CreateDefaultSubobject<UChaosWheeledVehicleMovementComponent>(TEXT("ChaosWheeledVehicleMovementComponent"));
	
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
