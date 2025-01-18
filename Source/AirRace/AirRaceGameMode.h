// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SplineActor.h"
#include "StraightRoadActor.h"
#include "GameFramework/GameModeBase.h"
#include "AirRaceGameMode.generated.h"

UCLASS(minimalapi)
class AAirRaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAirRaceGameMode();
	
	// Function to trigger Game Over
	UFUNCTION(BlueprintCallable)
	void GameOver();
	
	// Function to restart the game
	UFUNCTION(BlueprintCallable)
	void RestartGame();


};
