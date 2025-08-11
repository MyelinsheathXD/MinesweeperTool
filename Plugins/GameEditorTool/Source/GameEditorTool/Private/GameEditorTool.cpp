// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameEditorTool.h"
#include "GameEditorToolStyle.h"
#include "GameEditorToolCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName GameEditorToolTabName("GameEditorTool");

#define LOCTEXT_NAMESPACE "FGameEditorToolModule"

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
}

TSharedRef<SDockTab> FGameEditorToolModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FGameEditorToolModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("GameEditorTool.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
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
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FGameEditorToolCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGameEditorToolModule, GameEditorTool)