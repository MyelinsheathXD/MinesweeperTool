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
#include "ToolMenus.h"
#include "Styling/CoreStyle.h"
#include "Input/Reply.h"
#include "Framework/Commands/UIAction.h"

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
			/*FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(
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
				);
			}*/
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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameEditorToolModule, GameEditorTool)