##########################################importing########################

import copy
from typing import List, Optional, Tuple

####################################class Boggle_board##############################

class BoggleBoard:
    def __init__(self, data: List[List[str]]):
        self.__board: List[List[str]]] = copy.deepcopy(data)

    def cell_list(self) -> List[Tuple[int]]:
        """return all the cordinates of cells in the board"""
        cell_list = []
        for i in range(len(self.__board)):
            for j in range(len(self.__board[i])):
                cell_list.append((i, j))
        return cell_list

    def num_cells(self) -> int:
        """return the number of cells in the board"""
        res = 0
        for i in range(len(self.__board)):
            for j in range(len(self.__board[i])):
                res += 1
        return res

    def cell_content(self, cell: Tuple[int]) -> str:
        """return the content of given cell"""
        return self.__board[cell[0]][cell[1]]

    def paths_word(self, path: List[Tuple[int]]) -> str:
        """return the letters in given path"""
        res = ""
        for cell in path:
            res += self.cell_content(cell)
        return res

    def get_board(self) -> List[List[str]]]:
        """return the board"""
        return self.__board
