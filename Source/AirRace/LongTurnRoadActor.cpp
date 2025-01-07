// Fill out your copyright notice in the Description page of Project Settings.


#include "LongTurnRoadActor.h"

ALongTurnRoadActor::ALongTurnRoadActor()
{
	// RandomForm = 0.0f;
	SplinePoints = 10 + (20 * RandomForm);
	Radius = 1000 + (1000 * RandomForm);
	DegreeStep = 90 / SplinePoints;
	
	// GenerateSplinePoints();
}
