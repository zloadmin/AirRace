// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
struct AIRRACE_API FRoadActorParams
{
	// Splines 
	int SplinePoints = 5;
	int SplinePointsOffset = 2;
	int SplineDistanceBetweenPoints = 1000;

	// Circle 
	double CircleAngle = 0.0f;
	double CircleRadius  = 0.0f;
	double CircleTurnSide = 1.0f; // positive left, negative right;
	double CircleDegreeStep = 0.0f;
	
	
};
