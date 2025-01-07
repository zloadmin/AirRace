// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbRoadActor.h"

AClimbRoadActor::AClimbRoadActor()
{
	
	SplinePoints = 10 + (20 * RandomForm);
	ClimbAngle = 10 + (20 * RandomForm);
	SplinePointClimbStart = 0;
	SplinePointClimbEnd = SplinePoints ;
	//GenerateSplinePoints();
}
