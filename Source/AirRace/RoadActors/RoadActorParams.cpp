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

void FRoadActorParams::SetCircleDegree(const double& NewCircleDegree)
{
	CircleDegree = NewCircleDegree;
}

void FRoadActorParams::SetClimbAngle(const double& NewClimbAngle)
{
	ClimbAngle = NewClimbAngle;
}

void FRoadActorParams::SetSplinePoints(const int& NewSplinePoints)
{
	SplinePoints = NewSplinePoints;
}

void FRoadActorParams::SetCircleAngleAfterRound(const int& NewCircleAngleAfterRound)
{
	CircleAngleAfterRound = NewCircleAngleAfterRound;
}

void FRoadActorParams::SetRoll(const double& NewRoll)
{
	Roll = NewRoll;
}

int FRoadActorParams::GetSplinePoints() const
{
	return SplinePoints;
}

// int FRoadActorParams::GetSplinePointsWithoutOffset() const
// {
// 	return SplinePoints - (SplinePointsOffset * 2);
// }

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


double FRoadActorParams::GetCircleDegree() const
{
	return CircleDegree;
}

double FRoadActorParams::GetClimbAngle() const
{
	return ClimbAngle;
}

double FRoadActorParams::GetRoll() const
{
	return Roll;
}

bool FRoadActorParams::HasRoll() const
{
	return Roll != 0.0f;
}

int FRoadActorParams::GetCircleAngleAfterRound() const
{
	return CircleAngleAfterRound;
}

FRoadActorParams FRoadActorParams::SimpleStraight()
{
	return FRoadActorParams();
}

FRoadActorParams FRoadActorParams::SimpleStraightWithRoll()
{
	FRoadActorParams SimpleStraightWithRoll;
	SimpleStraightWithRoll.SetSplinePoints(30);
	SimpleStraightWithRoll.SetRoll(-15.0f);
	return SimpleStraightWithRoll;
}

FRoadActorParams FRoadActorParams::StraightWithRoll()
{
	FRoadActorParams StraightWithRoll;
	StraightWithRoll.SetRoll(20.0f);
	return FRoadActorParams();
}

FRoadActorParams FRoadActorParams::SimpleClimb()
{
	FRoadActorParams SimpleClimb;
	SimpleClimb.SetClimbAngle(200);
	SimpleClimb.SetSplinePoints(4);
	
	return SimpleClimb;
}

FRoadActorParams FRoadActorParams::SimpleTurn()
{
	FRoadActorParams SimpleTurn;

	SimpleTurn.SetCircleRadius(1000.0f);
	SimpleTurn.SetCircleDegree(45.0f);
	return SimpleTurn;
}

FRoadActorParams FRoadActorParams::TurnOneEightZeroDegrees()
{
	FRoadActorParams TurnOneEightZeroDegrees;
	TurnOneEightZeroDegrees.SetSplinePoints(100);
	TurnOneEightZeroDegrees.SetCircleRadius(10000.0f);
	TurnOneEightZeroDegrees.SetCircleDegree(180.0f);
	TurnOneEightZeroDegrees.SetCircleAngleAfterRound(30);
	TurnOneEightZeroDegrees.SetRoll(-25.0f);
	return TurnOneEightZeroDegrees;
}

FRoadActorParams FRoadActorParams::TurnMinusOneEightZeroDegrees()
{
	FRoadActorParams TurnOneEightZeroDegrees;
	TurnOneEightZeroDegrees.SetSplinePoints(100);
	TurnOneEightZeroDegrees.SetCircleRadius(10000.0f);
	TurnOneEightZeroDegrees.SetCircleDegree(180.0f);
	TurnOneEightZeroDegrees.SetCircleTurnSide(TURN_RIGHT);
	TurnOneEightZeroDegrees.SetCircleAngleAfterRound(30);
	TurnOneEightZeroDegrees.SetRoll(25.0f);
	return TurnOneEightZeroDegrees;
}

FRoadActorParams FRoadActorParams::TurnNineZeroDegrees()
{
	FRoadActorParams SimpleTurn;
	SimpleTurn.SetSplinePoints(50);
	SimpleTurn.SetCircleRadius(10000.0f);
	SimpleTurn.SetCircleDegree(90.0f);
	SimpleTurn.SetCircleAngleAfterRound(30);
	return SimpleTurn;
}

FRoadActorParams FRoadActorParams::SimpleTurnLeft()
{
	FRoadActorParams SimpleTurnLeft;
	SimpleTurnLeft.SetCircleRadius(1000.0f);
	SimpleTurnLeft.SetCircleDegree(45.0f);
	return SimpleTurnLeft;
}

FRoadActorParams FRoadActorParams::SimpleTurnRight()
{
	FRoadActorParams SimpleTurnRight;
	SimpleTurnRight.SetCircleRadius(1000.0f);
	SimpleTurnRight.SetCircleDegree(45.0f);
	SimpleTurnRight.SetCircleTurnSide(TURN_RIGHT);
	return SimpleTurnRight;
}

FRoadActorParams FRoadActorParams::SharplyTurnRight()
{
	FRoadActorParams SimpleTurnRight;
	SimpleTurnRight.SetCircleRadius(1000.0f);
	SimpleTurnRight.SetCircleDegree(45.0f);
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
