// Fill out your copyright notice in the Description page of Project Settings.


#include "AirRaceGameState.h"

#include "StraightRoadActor.h"

AAirRaceGameState::AAirRaceGameState()
{
	
	GenerateNextRoad();
	GenerateNextRoad();
	GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	
	
	// GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("AAirRaceGameState::AAirRaceGameState")));
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	
}

void AAirRaceGameState::GenerateNextRoad()
{
	UWorld* const World = GetWorld();
	if(World != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Add road")));
		
		AStraightRoadActor* ResultPawn = World->SpawnActor<AStraightRoadActor>(
			AStraightRoadActor::StaticClass(),
			 GetRoadFinish(),
			FRotator(0, 0, 0)
		);
		Roads.Add(ResultPawn);
		SetRoadFinish(ResultPawn);
	}
	
}

void AAirRaceGameState::BeginPlay()
{
	Super::BeginPlay();
	GenerateNextRoad();
}

FVector AAirRaceGameState::GetRoadFinish()
{
	return RoadFinish;
}

void AAirRaceGameState::SetRoadFinish(ASplineActor* Actor)
{
	RoadFinish = Actor->GetFinish();	
}
