// Fill out your copyright notice in the Description page of Project Settings.

#include "AirRaceGameState.h"


void AAirRaceGameState::InitRoad()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		Road = World->SpawnActor<ARoadActor>(ARoadActor::StaticClass(), FVector(), FRotator());
		Road->Add(FRoadActorParams());
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
