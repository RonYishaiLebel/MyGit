# Snake Game Project

## Overview
This project implements a version of the classic **Snake Game** using Python. The game features a graphical user interface (GUI) developed with `tkinter`, and includes game mechanics for snake movement, collision detection, and scoring. Players control the snake, collect apples, and avoid walls while trying to achieve the highest score possible.

## Files
1. **`game_display.py`**: Manages the game's GUI and handles user input, such as key presses for controlling the snake.
2. **`snake_game.py`**: Contains the main game logic, including the snake's movement, apple collection, and wall collision.
3. **`game_utils.py`**: Provides utility functions to generate random apple and wall positions, manage game settings, and set up the game board.
4. **`snake_main.py`**: Initializes the game and runs the main game loop, integrating the display and game logic.
5. **`score.py`**: Displays and updates the player's score during gameplay.
6. **`buttons.py`**: Defines interactive buttons for the GUI.
7. **`timer.py`**: Manages the countdown timer during gameplay.

## Running the Game
To run the game, execute the following command in your terminal:
```bash
python3 game_display.py
```

## Game Rules
- The game board is a grid where a snake moves around to collect apples.
- The goal is to grow the snake by eating apples, which increases the snake’s length.
- The snake can move in four directions: up, down, left, and right.
- The game ends if the snake hits a wall or collides with its own body.
- The player’s score increases based on how many apples the snake collects.

## Game Rules
- The game board is a grid where a snake moves around to collect apples.
- The goal is to grow the snake by eating apples, which increases the snake’s length.
- The snake can move in four directions: up, down, left, and right.
- The game ends if the snake hits a wall or collides with its own body.
- The player’s score increases based on how many apples the snake collects.

## Core Functionalities
### `Snake.move_snake(move_key, extending)`
- **Description**: Moves the snake in the given direction and handles snake growth if it eats an apple.
- **Parameters**:
  - `move_key`: The direction in which the snake should move (Up, Down, Left, Right).
  - `extending`: Boolean indicating whether the snake should grow (after eating an apple).

### `SnakeGame.update_objects()`
- **Description**: Updates the game state by moving the snake, checking for apple collection, and handling collisions.
- **Returns**: A tuple indicating whether any apples or walls were removed, and whether the snake ate an apple.

### `GameDisplay.draw_cell(x, y, color)`
- **Description**: Draws a cell on the game board at the specified (x, y) location and fills it with the specified color.
- **Parameters**:
  - `x`, `y`: Coordinates of the cell to be drawn.
  - `color`: The color to fill the cell with (e.g., "green" for apples, "black" for the snake).
  
### `GameDisplay.get_key_clicked()`
- **Description**: Returns the direction key clicked by the player, allowing the game to update the snake's direction.
- **Returns**: One of 'Up', 'Down', 'Left', or 'Right'.

## Graphical User Interface (GUI)
- The game is displayed on a grid with each cell representing a part of the game (snake, apples, or empty space).
- The player can control the snake's movement by pressing the arrow keys.
- Apples are represented in green, and the snake is represented in black.
- The game includes a real-time score display and a timer showing how long the game has been running.
- The game ends when the snake collides with a wall or itself, at which point the player can choose to play again or exit.
