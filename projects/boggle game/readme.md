# Boggle Game Project

## Overview
This project implements a version of the **Boggle Game** using Python. It features a graphical user interface (GUI) built with the `tkinter` module, along with several essential functionalities to manage the game logic and user interactions.

## Files
1. **`boggle.py`**: The main script that starts the game. It sets up the Boggle board, manages user input, and displays the GUI.
2. **`BoggleGame.py`**: Contains the game logic, including handling player actions, tracking the score, and updating the board.
3. **`BoggleBoard.py`**: Manages the structure of the Boggle board, keeping track of letters and paths.
4. **`boggle_board_randomizer.py`**: Provides functionality to generate a random 4x4 Boggle board for gameplay.
5. **`BoggleDisplay.py`**: Implements the graphical interface, showing the board and facilitating player interactions.
6. **`score.py`**: Manages the score display and updates based on player actions.
7. **`buttons.py`**: Defines the interactive letter buttons used in the GUI.
8. **`used_words.py`**: Tracks and displays the words that have already been found during the game.
9. **`boggle_dict.txt`**: A text file containing a list of valid words used for gameplay validation.

## Running the Game
To start the game, execute the following command in your terminal:
```bash
python boggle.py
```


## Game Rules
- The game board is a 4x4 grid filled with random letters.
- Players have 3 minutes to find as many valid words as they can.
- Words must be formed by connecting adjacent letters on the board, including diagonally.
- A letter can be used only once per word but may be reused in different words.
- The letter combination "QU" is treated as a single letter for word formation.

## Core Functionalities
### `BoggleGame.pass_input(path)`
- **Description**: Checks if a selected path on the board corresponds to a valid word and updates the game state accordingly.
- **Parameters**:
  - `path`: A list of tuples representing the path of selected letters.
- **Returns**: The valid word if found, otherwise returns an error message.

### `BoggleBoard.cell_content(loc)`
- **Description**: Retrieves the content of a specific cell on the board.
- **Parameters**:
  - `loc`: A tuple representing the cell coordinates.
- **Returns**: A list of objects (e.g., Apple, Wall) at the specified location or `None`.

### `BoggleBoard.get_board()`
- **Description**: Returns the current state of the Boggle board.
- **Returns**: A 4x4 list of lists containing the letters on the board.

### `boggle_board_randomizer.randomize_board()`
- **Description**: Generates a new random 4x4 Boggle board.
- **Returns**: A 4x4 list containing randomly selected letters.

## Graphical User Interface (GUI)
- The game board is presented as a 4x4 grid of buttons representing letters.
- Players can form words by clicking on the letter buttons.
- Selected words are highlighted on the board for visibility.
- The game begins when the player clicks the "Start" button.
- A countdown timer starts at 3 minutes.
- The score and list of found words are displayed during gameplay.
- At the end of the game, players have the option to restart or exit.
