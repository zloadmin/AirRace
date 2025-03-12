// Fill out your copyright notice in the Description page of Project Settings.

#include "AirRaceGameState.h"


void AAirRaceGameState::InitRoad()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		Road = World->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), FVector(0.0f, 0.0f, 1000.0f), FRotator());

		// Road->Add(FRoadActorParams::TurnOneEightZeroDegrees());

		
		// Road->Add(FRoadActorParams::SimpleStraight());
		// Road->Add(FRoadActorParams::SimpleStraight());
		Road->Add(FRoadActorParams::SimpleClimb());
		Road->Add(FRoadActorParams::SimpleStraight());
		// Road->Add(FRoadActorParams::SimpleStraight());
		Road->Add(FRoadActorParams::TurnMinusOneEightZeroDegrees());
		// Road->Add(FRoadActorParams::SimpleStraight());
		// Road->Add(FRoadActorParams::TurnOneEightZeroDegrees());
		// Road->Add(FRoadActorParams::TurnOneEightZeroDegrees());
		// Road->Add(FRoadActorParams::SimpleStraight());
		// Road->Add(FRoadActorParams::SimpleStraight());
		// Road->Add(FRoadActorParams::SimpleStraight());
		// Road->Add(FRoadActorParams::SimpleStraight());
		// Road->Add(FRoadActorParams::Random());
		// Road->Add(FRoadActorParams::Random());
		// Road->Add(FRoadActorParams::Random());
		// Road->Add(FRoadActorParams::Random());
		// Road->Add(FRoadActorParams::Random());
		// Road->Add(FRoadActorParams::SimpleTurnLeft());
		// Road->Add(FRoadActorSimpleTurnParams());
		// Road->Add();
		// Road->Add();
	}
}


FVector AAirRaceGameState::GetRoadSplinePointLocation(const int& SplinePointIndex)
{
	return Road->Spline->GetLocationAtSplinePoint(SplinePointIndex, ESplineCoordinateSpace::World);
}

ARoadActor* AAirRaceGameState::GetRoadActor()
{
	return Road;
}

void AAirRaceGameState::BeginPlay()
{
	Super::BeginPlay();
	InitRoad();
}

void AAirRaceGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
