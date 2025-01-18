// Copyright Epic Games, Inc. All Rights Reserved.

#include "AirRaceGameMode.h"
#include "AirRaceCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "AirRaceGameState.h"
#include "AirRaceHUD.h"
#include "AirRacePlayerController.h"
#include "BaseSportsCarPawn.h"
#include "Kismet/GameplayStatics.h"

AAirRaceGameMode::AAirRaceGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	// DefaultPawnClass = PlayerPawnClassFinder.Class;
	DefaultPawnClass = ABaseSportsCarPawn::StaticClass();
	GameStateClass = AAirRaceGameState::StaticClass();
	HUDClass = AAirRaceHUD::StaticClass();
	PlayerControllerClass = AAirRacePlayerController::StaticClass();
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
