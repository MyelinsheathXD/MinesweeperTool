// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinesweeperToolGameMode.h"
#include "MinesweeperToolCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMinesweeperToolGameMode::AMinesweeperToolGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
