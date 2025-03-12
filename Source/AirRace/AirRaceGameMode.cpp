// Copyright Epic Games, Inc. All Rights Reserved.

#include "AirRaceGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "AirRaceGameState.h"
#include "AirRaceHUD.h"
#include "AI/VehicleAIController.h"
#include "Cars/BaseSportsCarPawn.h"
#include "Kismet/GameplayStatics.h"

AAirRaceGameMode::AAirRaceGameMode()
	: Super()
{
	DefaultPawnClass = ABaseSportsCarPawn::StaticClass();
	GameStateClass = AAirRaceGameState::StaticClass();
	HUDClass = AAirRaceHUD::StaticClass();
	// PlayerControllerClass = AVehicleAIController::StaticClass();
	
}

void AAirRaceGameMode::GameOver()
{
	// Optionally pause the game
	auto PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->SetPause(true);
	}
}

void AAirRaceGameMode::RestartGame()
{

	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
