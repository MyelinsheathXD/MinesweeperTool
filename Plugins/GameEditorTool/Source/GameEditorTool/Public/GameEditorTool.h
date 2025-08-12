// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class STextBlock;

class FGameEditorToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<class SDockTab> OnSpawnMinesweeperTab(const class FSpawnTabArgs& SpawnTabArgs);
	
	/** Force refresh the minesweeper display */
	void RefreshMinesweeperDisplay();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	
	// Minesweeper dimensions
	int32 MinesweeperWidth;
	int32 MinesweeperHeight;
	
	// Reference to the current values text block for updating
	TSharedPtr<STextBlock> CurrentValuesTextBlock;
};
