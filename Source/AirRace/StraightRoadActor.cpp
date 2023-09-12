// Fill out your copyright notice in the Description page of Project Settings.


#include "StraightRoadActor.h"

AStraightRoadActor::AStraightRoadActor()
{
	Radius = 0; Roll = 0;
	SplinePoints = 10 + RandomForm / 5;
}
