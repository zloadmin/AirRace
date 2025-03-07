// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define TURN_LEFT 1.0f
#define TURN_RIGHT -1.0f

struct AIRRACE_API FRoadActorParams
{
	// explicit FRoadActorParams(
	// 	const int SplinePoints = 5,
	// 	const int SplinePointsOffset = 2,
	// 	const int SplineDistanceBetweenPoints = 1000,
	// 	const double CircleAngle = 0.0f,
	// 	const double CircleRadius = 0.0f,
	// 	const double CircleTurnSide = 1.0f,
	// 	const double CircleDegreeStep = 0.0f
	// ) : SplinePoints(SplinePoints),
	//     SplinePointsOffset(SplinePointsOffset),
	//     SplineDistanceBetweenPoints(SplineDistanceBetweenPoints),
	//     CircleAngle(CircleAngle),
	//     CircleRadius(CircleRadius),
	//     CircleTurnSide(CircleTurnSide),
	//     CircleDegreeStep(CircleDegreeStep)
	// {
	// }

private:
	// Splines 
	int SplinePoints = 6;
	int SplinePointsOffset = 2;
	int SplineDistanceBetweenPoints = 1000;

	// Circle 
	double CircleRadius = 0.0f;
	double CircleTurnSide = TURN_LEFT;
	double CircleDegree = 0.0f;
	int CircleAngleAfterRound = 0;

	// Roll
	double Roll = 0.0f;

	// enum ERollType
	// {
	// 	None, Center
	// };

private:
	// Climb
	double ClimbAngle = 0.0f;


	void SetCircleRadius(const double& NewCircleRadius);
	void SetCircleTurnSide(const double& NewCircleTurnSide);
	void SetCircleDegree(const double& NewCircleDegree);
	void SetClimbAngle(const double& NewClimbAngle);
	void SetSplinePoints(const int& NewSplinePoints);
	void SetCircleAngleAfterRound(const int& NewCircleAngleAfterRound);
	void SetRoll(const double& NewRoll);

public:
	// Splines 
	int GetSplinePoints() const;
	// int GetSplinePointsWithoutOffset() const;
	int GetSplinePointsOffset() const;
	int GetSplineDistanceBetweenPoints() const;
	double GetCircleRadius() const;
	double GetCircleTurnSide() const;
	double GetCircleDegree() const;
	double GetClimbAngle() const;
	double GetRoll() const;
	bool HasRoll() const;
	int GetCircleAngleAfterRound() const;

public:
	// Types
	static FRoadActorParams SimpleStraight();
	static FRoadActorParams SimpleStraightWithRoll();
	static FRoadActorParams StraightWithRoll();
	static FRoadActorParams SimpleClimb();
	static FRoadActorParams SimpleTurn();
	static FRoadActorParams TurnOneEightZeroDegrees();
	static FRoadActorParams TurnNineZeroDegrees();
	static FRoadActorParams SimpleTurnLeft();
	static FRoadActorParams SimpleTurnRight();
	static FRoadActorParams SharplyTurnRight();
	static FRoadActorParams Random();
};
