// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ChaosWheeledVehicleMovementComponent.h"

struct FTorqueCurveKeys
{
	float InTime;
	float InValue;
	const bool bUnwindRotation = false;
	FKeyHandle KeyHandle = FKeyHandle();
};

/**
 * 
 */

struct FCarSettings
{
	explicit FCarSettings(const int Level = 0);
	// General Setup
	// float MaxSpeed = 25500.0f;

	// Engine Setup
	float MaxRPM = 7500.0f;
	float MaxTorque = 1100.0f;
	FTorqueCurveKeys TorqueCurveKeys[3] = {
		{0.0f, 400.0f},
		{3000.0f, 800.0f},
		{7500.0f, 1100.0f},
	};
	// Transmission Setup
	bool bUseAutomaticGears = true;
	float GearChangeTime = 0.12f; // Fast gear shift
	float FinalRatio = 3.20f;
	float ForwardGearRatios[6] = {
		4.2f, 2.5f, 1.8f, 1.4f, 1.1f, 0.85,
	};
	// Differential Setup
	EVehicleDifferential DifferentialType = EVehicleDifferential::AllWheelDrive;
	float FrontRearSplit = 0.4f;

	// Suspension Setup
	float SuspensionRate = 400.0f; // 250.0f sport car
	float SuspensionDamping = 0.5f; // 0.5f sport car 
	float SuspensionPreload = 0.5f; // 0.05 sport car
	float SuspensionMaxRaise = 8.0f; // 10.0f sport car
	float SuspensionMaxDrop = 15.0f; // 10.0f sport car

	float FrontWheelRadius = 35.0f; // 39.0f
	float FrontWheelWidth = 20.0f; // 35.0f
	float FrontFrictionForceMultiplier = 2.0f; // 3.0f
	float FrontMaxSteerAngle = 35.0f; //  40.0f
	float FrontCorneringStiffness = 450.0f; // 1000.0f

	// float RearWheelRadius = 35.0f;
	// float RearWheelWidth = 22.0f;
	// float RearFriction = 2.2f;
	// float RearSteerAngle = 0.0f; // No rear-wheel steering
	// float RearSuspensionStiffness = 400.0f;

	
	// TArray<TPair<float, float>> TorqueCurveKeys = {
	// 	TPair<float, float>(0.0f, 400.0f),	
	// };
	// float SteeringSensitivity = 0.5f;
	// float BrakeForce = 5000.0f;
	// // float SuspensionStiffness = 50.0f;
	//
	// // GTA 5 Chat GPT settings
	// // Suspension Settings (Realistic but Soft)
	// float SuspensionStiffness = 50000.0f;  // Medium stiffness for a balanced ride
	// float SuspensionDamping = 3500.0f;  // Controls how fast suspension absorbs impacts
	// float SuspensionCompressionDamping = 4500.0f;  
	// float SuspensionRelaxationDamping = 4000.0f;
	// float SuspensionMaxRaise = 8.0f;  // Suspension extends slightly when airborne
	// float SuspensionMaxDrop = 8.0f;  // Prevents excessive body movement
	// float SuspensionLength = 35.0f;  // Adjust for different car types
	//
	// // Tire Grip & Friction
	// float WheelRadius = 35.0f;  // Standard car wheel size
	// float WheelWidth = 20.0f;  // Wide tires for grip
	// float FrictionMultiplier = 1.5f;  // Controls how "sticky" the tires are
	// float SideSlipModifier = 1.0f;  // Affects drifting behavior (lower = more slide)
	// float CorneringStiffness = 1500.0f;  // Balance between grip & responsiveness
	//
	// // Steering & Handling
	// float MaxSteerAngle = 35.0f;  // Similar to GTA 5 turning radius
	// float SteerAngleRatio = 0.9f;  // Smooth but quick steering response
	// float AckermannRatio = 0.5f;  // Better turn radius balance
	// float bAffectedByHandbrake = true;  // Allow drifting with handbrake
	// float HandbrakeTorque = 2500000.0f;  // Moderate handbrake effect
	//
	// // Braking & Acceleration
	// float BrakeTorque = 300000.0f;  // Responsive braking
	// float MaxBrakeTorque = 500000.0f;  // Stronger braking at high speeds
	// float EngineBrakeEffect = 0.5f;  // Slight engine braking for realism
	//
	// // Miscellaneous
	// float Mass = 20.0f;  // Standard wheel mass
	// float bUsePhysicsCollision = false;  // Disable extra physics on the wheel
};
