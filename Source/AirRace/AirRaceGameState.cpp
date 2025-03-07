// Fill out your copyright notice in the Description page of Project Settings.

#include "AirRaceGameState.h"



void AAirRaceGameState::InitRoad()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		Road = World->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), FVector(0.0f, 0.0f, 1000.0f), FRotator());
		Road->Add(FRoadActorParams::TurnOneEightZeroDegrees());
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


void AAirRaceGameState::BeginPlay()
{
	Super::BeginPlay();
	InitRoad();
}

void AAirRaceGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
