##########IMPORTING##########

from BoggleBoard import BoggleBoard
from typing import List, Optional, Tuple, Set
import ex11_utils as EU
import boggle_board_randomizer as bbr
import BoggleDisplay as gd

#########CONSTANTS#########

INITIAL_SCORE: int= 0
GAME_TIME: int = 180
USED_WORD_MSG: str= "you have already used this word!!!"

################## BOGGLE-GAME CLASS ###################

class BoggleGame:
    def __init__(self, board: BoggleBoard, words: List[str]) -> None:
        self.__board: BoggleBoard = board
        self.__words: list[str] = words
        self.__score: int = INITIAL_SCORE
        self.__time: int = GAME_TIME
        self.__used_words: List[str] = []

    def get_words(self) -> List[str]:
        """return the dictionary of words"""
        return self.__words

    def get_score(self) -> int:
        """return current score"""
        return self.__score

    def get_board(self) -> BoggleBoard:
        """return the board of letters"""
        return self.__board

    def get_time(self) -> int:
        """return the time the game should run"""
        return self.__time

    def get_used_words(self) -> str:
        """return the word that were already found"""
        return ", ".join(word for word in self.__used_words)

    def set_new_board(self) -> None:
        """init a new board with letters"""
        board_data = BoggleBoard(bbr.randomize_board())
        self.__board = board_data
        return

    def reset_score(self) -> None:
        """restart the score to 0"""
        self.__score = 0
        return

    def pass_input(self, input_path: List[Tuple[int, int]]) -> Optional[str]:
        """return an error message if needed ele update the words that were found and the score"""
        valid, word = EU.info_is_valid_path(
            self.__board.get_board(), input_path, self.__words)
        if not valid:
            return word
        if word in self.__used_words:
            return (USED_WORD_MSG)

        # if valid update words found and score
        self.__used_words.append(word)
        self.__score += len(word)**2
        return

    def reset_used_words(self) -> None:
        """restart the list of words already found"""
        self.__used_words = []
