// Fill out your copyright notice in the Description page of Project Settings.


#include "StraightRoadActor.h"

AStraightRoadActor::AStraightRoadActor() 
{
	// SplinePoints = 10 + (20 * RandomForm);
	SplinePoints = 10;
	GenerateSplinePoints();
}
