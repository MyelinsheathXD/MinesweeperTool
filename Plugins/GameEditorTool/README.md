# MinesweeperTool Plugin

This Unreal Engine plugin adds a Minesweeper Tool window to the editor with two integer input fields for width and height.

## Features

- **Width Input Field**: Accepts integer values from 1 to 100 for the minesweeper grid width
- **Height Input Field**: Accepts integer values from 1 to 100 for the minesweeper grid height
- **Real-time Updates**: Shows current width x height values and their multiplication below the input fields
- **Live Calculation**: Displays the total grid size (width × height) that updates automatically when you change values
- **Multiple Access Points**: Available through menu, toolbar, and main plugin window
- **Refresh Button**: Manual refresh button to update the display if needed

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

The current values are displayed below the input fields in the format "Current: WidthxHeight = TotalGridSize".

**Example**: If you set Width = 10 and Height = 15, it will show "Current: 10x15 = 150"

## Real-time Updates

- Values update automatically as you type or use the spin box arrows
- The multiplication (width × height) is calculated and displayed in real-time
- No need to click refresh - changes are immediate
- A refresh button is available for manual updates if needed

## Technical Details

- Built using Unreal Engine's Slate UI framework
- Uses `SSpinBox<int32>` for integer input with validation
- Values are stored in the plugin module for persistence during the editor session
- Default values: Width = 10, Height = 10
- Real-time display updates using shared widget references

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
- Grid preview visualization

