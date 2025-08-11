// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameEditorToolCommands.h"

#define LOCTEXT_NAMESPACE "FGameEditorToolModule"

void FGameEditorToolCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "GameEditorTool", "Bring up GameEditorTool window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
