// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "GameEditorToolStyle.h"

class FGameEditorToolCommands : public TCommands<FGameEditorToolCommands>
{
public:

	FGameEditorToolCommands()
		: TCommands<FGameEditorToolCommands>(TEXT("GameEditorTool"), NSLOCTEXT("Contexts", "GameEditorTool", "GameEditorTool Plugin"), NAME_None, FGameEditorToolStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};