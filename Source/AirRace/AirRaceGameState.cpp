// Fill out your copyright notice in the Description page of Project Settings.


#include "AirRaceGameState.h"

#include "ClimbRoadActor.h"
#include "LongTurnRoadActor.h"
#include "StraightRoadActor.h"
#include "Math/UnrealMathUtility.h"

AAirRaceGameState::AAirRaceGameState()
{
	// SetActors();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
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
			GetRoadRotation()
		);
		// Roads.Add(ResultPawn);
		SetRoadFinish(SplineActor);
		SetRoadRotation(SplineActor);
		// SplineActor->LogSplinePointsLocations();
	}
}

void AAirRaceGameState::BeginPlay()
{
	Super::BeginPlay();
	SetActors();
	GenerateNextRoad();
	GenerateNextRoad();
	GenerateNextRoad();
	GEngine->Exec(GetWorld(), TEXT("stat fps"));
	// GenerateNextRoad();
	// GenerateNextRoad();
	// GenerateNextRoad();
}

FVector AAirRaceGameState::GetRoadFinish()
{
	return RoadFinish;
}

FRotator AAirRaceGameState::GetRoadRotation()
{
	return RoadRotation;
}

void AAirRaceGameState::SetRoadFinish(ASplineActor* Actor)
{
	RoadFinish = Actor->GetFinish();
	UE_LOG(LogTemp, Warning, TEXT("Set road finish %s"), *Actor->GetFinish().ToString());
}

void AAirRaceGameState::SetRoadRotation(ASplineActor* Actor)
{
	RoadRotation = Actor->GetRotation();
	// RoadRotation = FRotator(Actor->GetRotation().Pitch, Actor->GetRotation().Yaw, Actor->GetRotation().Roll - 90.f);
}

void AAirRaceGameState::SetActors()
{
	// Actors.Add(AStraightRoadActor::StaticClass());
	Actors.Add(AStraightRoadActor::StaticClass());
	// Actors.Add(ALongTurnRoadActor::StaticClass());
}

UClass* AAirRaceGameState::GetRandomActor()
{
	return Actors[FMath::RandRange(0, Actors.Num() - 1)];
}
