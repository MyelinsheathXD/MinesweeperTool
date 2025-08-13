// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameEditorTool.h"
#include "GameEditorToolStyle.h"
#include "GameEditorToolCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SSpinBox.h"
/*#include "Widgets/Layout/SVerticalBox.h"
#include "Widgets/Layout/SHorizontalBox.h"*/
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "ToolMenus.h"
#include "Styling/CoreStyle.h"
#include "Input/Reply.h"
#include "Framework/Commands/UIAction.h"
#include "Math/UnrealMathUtility.h"

static const FName GameEditorToolTabName("GameEditorTool");

#define LOCTEXT_NAMESPACE "FGameEditorToolModule"


static const FName MinesweeperTabName("MinesweeperTool");

void FGameEditorToolModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGameEditorToolStyle::Initialize();
	FGameEditorToolStyle::ReloadTextures();

	FGameEditorToolCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGameEditorToolCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FGameEditorToolModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGameEditorToolModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GameEditorToolTabName, FOnSpawnTab::CreateRaw(this, &FGameEditorToolModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FGameEditorToolTabTitle", "GameEditorTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
		
	// Register MinesweeperTool tab
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(MinesweeperTabName, FOnSpawnTab::CreateRaw(this, &FGameEditorToolModule::OnSpawnMinesweeperTab))
		.SetDisplayName(LOCTEXT("MinesweeperToolTabTitle", "MinesweeperTool"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
		
	// Initialize default values
	MinesweeperWidth = 10;
	MinesweeperHeight = 10;
	

}

void FGameEditorToolModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGameEditorToolStyle::Shutdown();

	FGameEditorToolCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GameEditorToolTabName);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(MinesweeperTabName);
}

TSharedRef<SDockTab> FGameEditorToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("GameEditorToolTitle", "Game Editor Tool"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(SButton)
				.Text(LOCTEXT("OpenMinesweeperButton", "Open Minesweeper Tool"))
				.OnClicked_Lambda([this]() -> FReply
				{
					FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
					return FReply::Handled();
				})
			]
		];
}

TSharedRef<SDockTab> FGameEditorToolModule::OnSpawnMinesweeperTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MinesweeperTitle", "Minesweeper Tool"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("WidthLabel", "Width:"))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5)
				[
					SNew(SSpinBox<int32>)
					.MinValue(1)
					.MaxValue(100)
					.Value(MinesweeperWidth)
					.OnValueChanged_Lambda([this](int32 NewValue) 
					{ 
						MinesweeperWidth = NewValue; 
						RefreshMinesweeperDisplay();
					})
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("HeightLabel", "Height:"))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5)
				[
					SNew(SSpinBox<int32>)
					.MinValue(1)
					.MaxValue(100)
					.Value(MinesweeperHeight)
					.OnValueChanged_Lambda([this](int32 NewValue) 
					{ 
						MinesweeperHeight = NewValue; 
						RefreshMinesweeperDisplay();
					})
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(5)
				[
					SAssignNew(CurrentValuesTextBlock, STextBlock)
					.Text(FText::Format(LOCTEXT("CurrentValues", "Current: {0}x{1} = {2}"), 
						FText::AsNumber(MinesweeperWidth), 
						FText::AsNumber(MinesweeperHeight),
						FText::AsNumber(MinesweeperWidth * MinesweeperHeight)))
				]
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(SButton)
				.Text(LOCTEXT("RefreshButton", "Refresh Display"))
				.OnClicked_Lambda([this]() -> FReply
				{
					RefreshMinesweeperDisplay();
					return FReply::Handled();
				})
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(SButton)
				.Text(LOCTEXT("GenerateButton", "Generate Minesweeper"))
				.OnClicked_Lambda([this]() -> FReply
				{
					GenerateMinesweeperGame();
					return FReply::Handled();
				})
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SAssignNew(GameGridContainer, SVerticalBox)
			]
		];
}

void FGameEditorToolModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GameEditorToolTabName);
}

void FGameEditorToolModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FGameEditorToolCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}
	
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			/*FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntry(
				FToolMenuEntry::InitMenuEntry(
					"OpenMinesweeperTool",
					LOCTEXT("OpenMinesweeperTool", "Minesweeper Tool"),
					LOCTEXT("OpenMinesweeperToolTooltip", "Open the Minesweeper Tool window"),
					FSlateIcon(),
					FUIAction(FExecuteAction::CreateLambda([this]()
					{
						FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
					}))
				)
			);*/
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGameEditorToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
	
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				/*FToolMenuEntry& Entry = Section.AddEntry(
					FToolMenuEntry::InitToolBarButton(
						"OpenMinesweeperTool",
						LOCTEXT("OpenMinesweeperTool", "Minesweeper Tool"),
						LOCTEXT("OpenMinesweeperToolTooltip", "Open the Minesweeper Tool window"),
						FSlateIcon(),
						FUIAction(FExecuteAction::CreateLambda([this]()
						{
							FGlobalTabmanager::Get()->TryInvokeTab(MinesweeperTabName);
						}))
					)
				);*/
			}
		}
	}
}

void FGameEditorToolModule::RefreshMinesweeperDisplay()
{
	if (CurrentValuesTextBlock.IsValid())
	{
		CurrentValuesTextBlock->SetText(
			FText::Format(LOCTEXT("CurrentValues", "Current: {0}x{1} = {2}"), 
				FText::AsNumber(MinesweeperWidth), 
				FText::AsNumber(MinesweeperHeight),
				FText::AsNumber(MinesweeperWidth * MinesweeperHeight))
		);
	}
}

void FGameEditorToolModule::GenerateMinesweeperGame()
{
	// Calculate number of mines (about 15% of total cells)
	TotalMines = FMath::Max(1, FMath::RoundToInt((MinesweeperWidth * MinesweeperHeight) * 0.15f));
	
	// Initialize game grid
	GameGrid.SetNum(MinesweeperHeight);
	for (int32 Row = 0; Row < MinesweeperHeight; ++Row)
	{
		GameGrid[Row].SetNum(MinesweeperWidth);
		for (int32 Col = 0; Col < MinesweeperWidth; ++Col)
		{
			GameGrid[Row][Col] = FMinesweeperCell();
		}
	}
	
	// Place mines randomly
	int32 MinesPlaced = 0;
	while (MinesPlaced < TotalMines)
	{
		int32 Row = FMath::RandRange(0, MinesweeperHeight - 1);
		int32 Col = FMath::RandRange(0, MinesweeperWidth - 1);
		
		if (!GameGrid[Row][Col].bIsMine)
		{
			GameGrid[Row][Col].bIsMine = true;
			MinesPlaced++;
		}
	}
	
	// Calculate adjacent mine counts
	for (int32 Row = 0; Row < MinesweeperHeight; ++Row)
	{
		for (int32 Col = 0; Col < MinesweeperWidth; ++Col)
		{
			if (!GameGrid[Row][Col].bIsMine)
			{
				int32 AdjacentMines = 0;
				for (int32 dRow = -1; dRow <= 1; ++dRow)
				{
					for (int32 dCol = -1; dCol <= 1; ++dCol)
					{
						int32 CheckRow = Row + dRow;
						int32 CheckCol = Col + dCol;
						if (CheckRow >= 0 && CheckRow < MinesweeperHeight && 
							CheckCol >= 0 && CheckCol < MinesweeperWidth)
						{
							if (GameGrid[CheckRow][CheckCol].bIsMine)
							{
								AdjacentMines++;
							}
						}
					}
				}
				GameGrid[Row][Col].AdjacentMines = AdjacentMines;
			}
		}
	}
	
	// Reset game state
	bGameGenerated = true;
	bGameOver = false;
	bGameWon = false;
	RevealedCells = 0;
	
	// Refresh the game display
	if (GameGridContainer.IsValid())
	{
		//RefreshGameDisplay();
	}
}

void FGameEditorToolModule::OnCellClicked(int32 Row, int32 Col)
{
	if (!bGameGenerated || bGameOver || GameGrid[Row][Col].bIsFlagged)
		return;
	
	if (GameGrid[Row][Col].bIsMine)
	{
		// Game over - reveal all mines
		bGameOver = true;
		bGameWon = false;
		for (int32 r = 0; r < MinesweeperHeight; ++r)
		{
			for (int32 c = 0; c < MinesweeperWidth; ++c)
			{
				if (GameGrid[r][c].bIsMine)
				{
					GameGrid[r][c].bIsRevealed = true;
				}
			}
		}
	}
	else
	{
		// Reveal the cell
		RevealCell(Row, Col);
		
		// Check if game is won
		if (IsGameWon())
		{
			bGameOver = true;
			bGameWon = true;
		}
	}
	
	// Refresh the game display
	if (GameGridContainer.IsValid())
	{
		RefreshGameDisplay();
	}
}

void FGameEditorToolModule::OnCellRightClicked(int32 Row, int32 Col)
{
	if (!bGameGenerated || bGameOver || GameGrid[Row][Col].bIsRevealed)
		return;
	
	GameGrid[Row][Col].bIsFlagged = !GameGrid[Row][Col].bIsFlagged;
	
	// Refresh the game display
	if (GameGridContainer.IsValid())
	{
		RefreshGameDisplay();
	}
}

void FGameEditorToolModule::RevealCell(int32 Row, int32 Col)
{
	if (Row < 0 || Row >= MinesweeperHeight || Col < 0 || Col >= MinesweeperWidth)
		return;
	
	if (GameGrid[Row][Col].bIsRevealed || GameGrid[Row][Col].bIsFlagged)
		return;
	
	GameGrid[Row][Col].bIsRevealed = true;
	RevealedCells++;
	
	// If no adjacent mines, reveal neighbors
	if (GameGrid[Row][Col].AdjacentMines == 0)
	{
		for (int32 dRow = -1; dRow <= 1; ++dRow)
		{
			for (int32 dCol = -1; dCol <= 1; ++dCol)
			{
				RevealCell(Row + dRow, Col + dCol);
			}
		}
	}
}

bool FGameEditorToolModule::IsGameWon() const
{
	return RevealedCells == (MinesweeperWidth * MinesweeperHeight - TotalMines);
}

bool FGameEditorToolModule::IsGameLost() const
{
	return bGameOver && !bGameWon;
}

void FGameEditorToolModule::RefreshGameDisplay()
{
	if (!GameGridContainer.IsValid())
		return;
	
	// Clear existing game grid
	GameGridContainer->ClearChildren();
	
	// Add game status
	GameGridContainer->AddSlot()
		.AutoHeight()
		.Padding(5)
		[
			SNew(STextBlock)
			.Text_Lambda([this]() -> FText
			{
				if (!bGameGenerated)
					return LOCTEXT("NoGame", "No game generated yet");
				
				if (bGameOver)
				{
					if (bGameWon)
						return LOCTEXT("GameWon", "Congratulations! You won!");
					else
						return LOCTEXT("GameLost", "Game Over! You hit a mine!");
				}
				
				return FText::Format(LOCTEXT("GameStatus", "Mines: {0} | Revealed: {1}/{2}"), 
					FText::AsNumber(TotalMines),
					FText::AsNumber(RevealedCells),
					FText::AsNumber(MinesweeperWidth * MinesweeperHeight - TotalMines));
			})
		];
	
	// Add game grid
	for (int32 Row = 0; Row < MinesweeperHeight; ++Row)
	{
		TSharedRef<SHorizontalBox> RowBox = SNew(SHorizontalBox);
		
		for (int32 Col = 0; Col < MinesweeperWidth; ++Col)
		{
			const FMinesweeperCell& Cell = GameGrid[Row][Col];
			
			FText ButtonText;
			FLinearColor ButtonColor = FLinearColor::White;
			
			if (Cell.bIsFlagged)
			{
				ButtonText = LOCTEXT("Flag", "🚩");
				ButtonColor = FLinearColor::Red;
			}
			else if (Cell.bIsRevealed)
			{
				if (Cell.bIsMine)
				{
					ButtonText = LOCTEXT("Mine", "💣");
					ButtonColor = FLinearColor::Red;
				}
				else if (Cell.AdjacentMines > 0)
				{
					ButtonText = FText::AsNumber(Cell.AdjacentMines);
					// Different colors for different numbers
					switch (Cell.AdjacentMines)
					{
					case 1: ButtonColor = FLinearColor::Blue; break;
					case 2: ButtonColor = FLinearColor::Green; break;
					case 3: ButtonColor = FLinearColor::Red; break;
					case 4: ButtonColor = FLinearColor::Red; break;
					case 5: ButtonColor = FLinearColor::Red; break;
					case 6: ButtonColor = FLinearColor::Red; break;
					case 7: ButtonColor = FLinearColor::Black; break;
					case 8: ButtonColor = FLinearColor::Gray; break;
					default: ButtonColor = FLinearColor::Black; break;
					}
				}
				else
				{
					ButtonText = LOCTEXT("Empty", "");
					ButtonColor = FLinearColor::Gray;
				}
			}
			else
			{
				ButtonText = LOCTEXT("Hidden", "");
				ButtonColor = FLinearColor::Gray;
			}
			
			/*RowBox->AddSlot()
				.AutoWidth()
				.Padding(1)
				[
					SNew(SButton)
					.Text(ButtonText)
					.ButtonColorAndOpacity(ButtonColor)
					.OnClicked_Lambda([this, Row, Col]() -> FReply
					{
						OnCellClicked(Row, Col);
						return FReply::Handled();
					})
					.OnClicked_Lambda([this, Row, Col](const FGeometry&, const FPointerEvent&) -> FReply
					{
						OnCellRightClicked(Row, Col);
						return FReply::Handled();
					})
					.ContentPadding(FMargin(5))
					//.MinDesiredWidth(30)
					//.MinDesiredHeight(30)
				];*/

			
		}
		
		GameGridContainer->AddSlot()
			.AutoHeight()
			.Padding(1)
			[
				RowBox
			];
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameEditorToolModule, GameEditorTool)