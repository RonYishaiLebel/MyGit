######################################importing##############################

from BoggleBoard import BoggleBoard
import boggle_board_randomizer as bbr
import BoggleDisplay as gd
from BoggleGame import BoggleGame

#####################################main####################################

if __name__ == "__main__":
    words = []
    # go throgh the words in the txt file
    with open("boggle_dict.txt", 'r') as f:
        for line in f:
            for word in line.split():
                words.append(word)

    # create board, game and GUI and run the game
    board = BoggleBoard(bbr.randomize_board())
    game = BoggleGame(board, words)
    gd = gd.ColorBoard(game)
    gd.run()
