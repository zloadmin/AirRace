// Fill out your copyright notice in the Description page of Project Settings.


#include "CarSettings.h"

// UCarSettings::UCarSettings(const int Level)
// {
// 	MaxTorque = MaxTorque + (Level * 50.0f);
// 	MaxRPM = MaxRPM + (Level * 300.0f);
// 	// SteeringSensitivity = SteeringSensitivity + (Level * 0.05f);
// 	// BrakeForce = BrakeForce + (Level * 500.0f);
// 	// SuspensionStiffness = SuspensionStiffness + (SuspensionStiffness * 2.5f);	
// }

FCarSettings::FCarSettings(const int Level)
{
		MaxTorque = MaxTorque + (Level * 50.0f);
		MaxRPM = MaxRPM + (Level * 300.0f);
}
