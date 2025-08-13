// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;
class STextBlock;
class SButton;

// Minesweeper cell structure
struct FMinesweeperCell
{
	bool bIsMine = false;
	bool bIsRevealed = false;
	bool bIsFlagged = false;
	int32 AdjacentMines = 0;
};

class FGameEditorToolModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	void RefreshGameDisplay();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<class SDockTab> OnSpawnMinesweeperTab(const class FSpawnTabArgs& SpawnTabArgs);
	
	/** Force refresh the minesweeper display */
	void RefreshMinesweeperDisplay();
	
	/** Generate a new minesweeper game */
	void GenerateMinesweeperGame();
	
	/** Handle cell click in the minesweeper game */
	void OnCellClicked(int32 Row, int32 Col);
	
	/** Handle right-click (flag) in the minesweeper game */
	void OnCellRightClicked(int32 Row, int32 Col);
	
	/** Reveal a cell and its neighbors */
	void RevealCell(int32 Row, int32 Col);
	
	/** Check if the game is won */
	bool IsGameWon() const;
	
	/** Check if the game is lost */
	bool IsGameLost() const;

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	
	// Minesweeper dimensions
	int32 MinesweeperWidth;
	int32 MinesweeperHeight;
	
	// Reference to the current values text block for updating
	TSharedPtr<STextBlock> CurrentValuesTextBlock;
	
	// Minesweeper game data
	TArray<TArray<FMinesweeperCell>> GameGrid;
	bool bGameGenerated = false;
	bool bGameOver = false;
	bool bGameWon = false;
	int32 TotalMines = 0;
	int32 RevealedCells = 0;
	
	// Game UI references
	TSharedPtr<SVerticalBox> GameGridContainer;
};
