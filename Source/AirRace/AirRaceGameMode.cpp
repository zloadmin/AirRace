// Copyright Epic Games, Inc. All Rights Reserved.

#include "AirRaceGameMode.h"
#include "AirRaceCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAirRaceGameMode::AAirRaceGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
