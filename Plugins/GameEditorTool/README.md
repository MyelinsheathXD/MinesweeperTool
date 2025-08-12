# MinesweeperTool Plugin

This Unreal Engine plugin adds a Minesweeper Tool window to the editor with two integer input fields for width and height.

## Features

- **Width Input Field**: Accepts integer values from 1 to 100 for the minesweeper grid width
- **Height Input Field**: Accepts integer values from 1 to 100 for the minesweeper grid height
- **Real-time Updates**: Shows current width x height values below the input fields
- **Multiple Access Points**: Available through menu, toolbar, and main plugin window

## How to Use

### Method 1: Main Plugin Window
1. Go to **Window > GameEditorTool** in the main menu
2. Click the **"Open Minesweeper Tool"** button

### Method 2: Menu Access
1. Go to **Window > Minesweeper Tool** in the main menu

### Method 3: Toolbar Access
1. Look for the **Minesweeper Tool** button in the Level Editor toolbar

## Input Fields

- **Width**: Set the horizontal size of your minesweeper grid (1-100)
- **Height**: Set the vertical size of your minesweeper grid (1-100)

The current values are displayed below the input fields in the format "Current: WidthxHeight".

## Technical Details

- Built using Unreal Engine's Slate UI framework
- Uses `SSpinBox<int32>` for integer input with validation
- Values are stored in the plugin module for persistence during the editor session
- Default values: Width = 10, Height = 10

## Building

This plugin is designed to work with Unreal Engine 5. Make sure to:
1. Place the plugin in your project's `Plugins` folder
2. Rebuild the project
3. Enable the plugin in the project settings if needed

## Future Enhancements

- Add mine count input field
- Implement actual minesweeper game generation
- Add difficulty presets
- Save/load configuration options

