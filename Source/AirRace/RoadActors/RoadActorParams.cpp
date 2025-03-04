// Fill out your copyright notice in the Description page of Project Settings.
#include "RoadActorParams.h"


void FRoadActorParams::SetCircleRadius(const double& NewCircleRadius)
{
	CircleRadius = NewCircleRadius;
}

void FRoadActorParams::SetCircleTurnSide(const double& NewCircleTurnSide)
{
	CircleTurnSide = NewCircleTurnSide;
}

void FRoadActorParams::SetCircleDegreeStep(const double& NewCircleDegreeStep)
{
	CircleDegreeStep = NewCircleDegreeStep;
}

void FRoadActorParams::SetClimbAngle(const double& NewClimbAngle)
{
	ClimbAngle = NewClimbAngle;
}

int FRoadActorParams::GetSplinePoints() const
{
	return SplinePoints;
}

int FRoadActorParams::GetSplinePointsOffset() const
{
	return SplinePointsOffset;
}

int FRoadActorParams::GetSplineDistanceBetweenPoints() const
{
	return SplineDistanceBetweenPoints;
}

double FRoadActorParams::GetCircleRadius() const
{
	return CircleRadius;
}

double FRoadActorParams::GetCircleTurnSide() const
{
	return CircleTurnSide;
}


double FRoadActorParams::GetCircleDegreeStep() const
{
	return CircleDegreeStep;
}

double FRoadActorParams::GetClimbAngle() const
{
	return ClimbAngle;
}

FRoadActorParams FRoadActorParams::SimpleStraight()
{
	return FRoadActorParams();
}

FRoadActorParams FRoadActorParams::SimpleClimb()
{
	FRoadActorParams SimpleClimb;
	SimpleClimb.SetClimbAngle(100);
	return SimpleClimb;
}

FRoadActorParams FRoadActorParams::SimpleTurn()
{
	FRoadActorParams SimpleTurn;
	SimpleTurn.SetCircleRadius(1000.0f);
	SimpleTurn.SetCircleDegreeStep(90.0f / 10.0f);
	return SimpleTurn;
}

FRoadActorParams FRoadActorParams::SimpleTurnLeft()
{
	FRoadActorParams SimpleTurnLeft;
	SimpleTurnLeft.SetCircleRadius(1000.0f);
	SimpleTurnLeft.SetCircleDegreeStep(90.0f / 10.0f);
	return SimpleTurnLeft;
}

FRoadActorParams FRoadActorParams::SimpleTurnRight()
{
	FRoadActorParams SimpleTurnRight;
	SimpleTurnRight.SetCircleRadius(1000.0f);
	SimpleTurnRight.SetCircleDegreeStep(90.0f / 10.0f);
	SimpleTurnRight.SetCircleTurnSide(TURN_RIGHT);
	return SimpleTurnRight;
}

FRoadActorParams FRoadActorParams::SharplyTurnRight()
{
	FRoadActorParams SimpleTurnRight;
	SimpleTurnRight.SetCircleRadius(1000.0f);
	SimpleTurnRight.SetCircleDegreeStep(90.0f / 10.0f);
	SimpleTurnRight.SetCircleTurnSide(TURN_RIGHT);
	return SimpleTurnRight;
}


FRoadActorParams FRoadActorParams::Random()
{
	TArray<FRoadActorParams(*)()> Methods;
	Methods.Add(&FRoadActorParams::SimpleStraight);
	Methods.Add(&FRoadActorParams::SimpleTurn);
	Methods.Add(&FRoadActorParams::SimpleTurnLeft);
	Methods.Add(&FRoadActorParams::SimpleTurnRight);
	FRoadActorParams (*RandomMethod)() = Methods[FMath::RandRange(0, Methods.Num() - 1)];
	return (*RandomMethod)();
}
