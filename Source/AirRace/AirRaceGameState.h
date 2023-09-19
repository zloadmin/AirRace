// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SplineActor.h"
#include "GameFramework/GameStateBase.h"
#include "AirRaceGameState.generated.h"

/**
 * 
 */
UCLASS()
class AIRRACE_API AAirRaceGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	AAirRaceGameState();
	void GenerateNextRoad();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Road")
	FVector RoadFinish = FVector(0, 0, 0);
	UPROPERTY(VisibleAnywhere, Category = "Road")
	TArray<ASplineActor *> Roads;
private:
	FVector GetRoadFinish();
	void SetRoadFinish(ASplineActor* Actor);
	TArray<UClass *> Actors;
	void SetActors();
	UClass * GetRandomActor();
	
};
