"""
Boggle Game Main Script

This script imports necessary modules to initialize and run a game of Boggle.
It generates a Boggle board, reads a list of words from a text file,
creates a BoggleGame instance, initializes a graphical user interface (GUI)
for displaying the Boggle board, and runs the game.

Make sure to have the required modules (BoggleBoard, boggle_board_randomizer, BoggleDisplay, BoggleGame)
and the 'boggle_dict.txt' word list file in the same directory before running this script.

Usage:
    Run this script to start and play a game of Boggle.

"""

######################################importing##############################

from BoggleBoard import BoggleBoard
import boggle_board_randomizer as bbr
import BoggleDisplay as gd
from BoggleGame import BoggleGame

#####################################main####################################

if __name__ == "__main__":
    words = []
    
    # Load words from the txt file
    with open("boggle_dict.txt", 'r') as f:
        for line in f:
            for word in line.split():
                words.append(word)
    
    # Create a Boggle board, game instance, GUI, and run the game
    board = BoggleBoard(bbr.randomize_board())
    game = BoggleGame(board, words)
    gd = gd.ColorBoard(game)
    gd.run()
