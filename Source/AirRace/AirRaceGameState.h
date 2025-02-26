// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoadActors/RoadActor.h"
#include "GameFramework/GameStateBase.h"
#include "AirRaceGameState.generated.h"

/**
 * 
 */
UCLASS()
class AIRRACE_API AAirRaceGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
private:
	void InitRoad();
	ARoadActor* Road;
};
