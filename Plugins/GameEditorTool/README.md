# MinesweeperTool Plugin

This Unreal Engine plugin adds a Minesweeper Tool window to the editor with two integer input fields for width and height, plus a fully playable minesweeper game!

## Features

- **Width Input Field**: Accepts integer values from 1 to 100 for the minesweeper grid width
- **Height Input Field**: Accepts integer values from 1 to 100 for the minesweeper grid height
- **Real-time Updates**: Shows current width x height values and their multiplication below the input fields
- **Live Calculation**: Displays the total grid size (width × height) that updates automatically when you change values
- **Generate Minesweeper Button**: Creates a playable minesweeper game with the specified dimensions
- **Fully Playable Game**: Complete minesweeper gameplay with left-click to reveal, right-click to flag
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

## Playing Minesweeper

1. **Set Dimensions**: Use the width and height input fields to set your desired grid size
2. **Generate Game**: Click the **"Generate Minesweeper"** button to create a new game
3. **Game Rules**:
   - **Left-click** to reveal a cell
   - **Right-click** to place/remove a flag on suspected mine locations
   - Numbers show how many mines are adjacent to that cell
   - Empty cells automatically reveal neighboring cells
   - Avoid clicking on mines!

### Game Features

- **Smart Mine Placement**: Mines are randomly placed with about 15% density
- **Color-coded Numbers**: Different colors for different adjacent mine counts
- **Auto-reveal**: Empty cells automatically reveal safe neighbors
- **Win/Lose Conditions**: Game tracks victory and defeat states
- **Real-time Status**: Shows mine count, revealed cells, and game state

### Game Controls

- **Left Mouse Button**: Reveal cell
- **Right Mouse Button**: Toggle flag
- **Visual Feedback**: 
  - 🚩 = Flagged cell
  - 💣 = Mine (revealed on game over)
  - Numbers = Adjacent mine count
  - Empty = Safe cell with no adjacent mines

## Technical Details

- Built using Unreal Engine's Slate UI framework
- Uses `SSpinBox<int32>` for integer input with validation
- Values are stored in the plugin module for persistence during the editor session
- Default values: Width = 10, Height = 10
- Real-time display updates using shared widget references
- Complete minesweeper game logic with recursive cell revelation
- Responsive grid layout that adapts to any width/height combination

## Building

This plugin is designed to work with Unreal Engine 5. Make sure to:
1. Place the plugin in your project's `Plugins` folder
2. Rebuild the project
3. Enable the plugin in the project settings if needed

## Future Enhancements

- Add mine count input field for custom difficulty
- Add difficulty presets (Beginner, Intermediate, Expert)
- Save/load game configurations
- Grid preview visualization before generation
- Timer and score tracking
- Undo/redo functionality
- Custom themes and colors

