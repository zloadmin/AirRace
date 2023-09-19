// Fill out your copyright notice in the Description page of Project Settings.


#include "AirRaceGameState.h"

#include "ClimbRoadActor.h"
#include "LongTurnRoadActor.h"
#include "StraightRoadActor.h"
#include "Math/UnrealMathUtility.h"

AAirRaceGameState::AAirRaceGameState()
{
	SetActors();
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
	if (World != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, FString::Printf(TEXT("Add road")));

		ASplineActor* SplineActor = World->SpawnActor<ASplineActor>(
			GetRandomActor(),
			GetRoadFinish(),
			FRotator(0, 0, 0)
		);
		// Roads.Add(ResultPawn);
		SetRoadFinish(SplineActor);
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

void AAirRaceGameState::SetActors()
{
	Actors.Add(AStraightRoadActor::StaticClass());
	Actors.Add(AClimbRoadActor::StaticClass());
	// Actors.Add(ALongTurnRoadActor::StaticClass());
}

UClass* AAirRaceGameState::GetRandomActor()
{
	return Actors[FMath::RandRange(0, Actors.Num() - 1)];
}
