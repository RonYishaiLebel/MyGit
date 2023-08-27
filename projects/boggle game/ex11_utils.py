#################################importing#############################################

from typing import List, Tuple, Iterable, Optional
import copy
from BoggleBoard import *
import copy

#################################typing_constants########################################

Board = List[List[str]]
Path = List[Tuple[int, int]]


#################################helpers_funcs######################################

def squere(cell: Tuple[int]) -> List[Tuple[int]]:
    """this func recives a location on board, and returns all cells that circle it, even if not in board"""
    cell_list = []
    for i in range(cell[0] - 1, cell[0] + 2):
        for j in range(cell[1] - 1, cell[1] + 2):
            if cell != (i, j):
                cell_list.append((i, j))
    return cell_list


def _is_pattern_valid(board: BoggleBoard, path: Path) -> bool:
    """checks if a pattern is valid, according to the games rulles"""
    if len(set(path)) != len(path):
        return False
    for i in range(len(path)):
        if path[i] not in board.cell_list():
            return False
    for i in range(len(path)-1):
        if path[i+1] not in squere(path[i]) or path[i] not in board.cell_list():
            return False
    return True


def load_words_dict(file):
    milon = open(file)
    lines = set(line.strip() for line in milon.readlines())
    milon.close()
    return lines

##################################is_valid_path##############################################

def is_valid_path(board: Board, path: Path, words: Iterable[str]) -> Optional[str]:
    """in addition to _is_pattern_valid checks, checkes if the the word is legal
    if legal returns the word"""
    result = ""
    boogle_board = BoggleBoard(board)
    if _is_pattern_valid(boogle_board, path):
        for cell in path:
            result += boogle_board.cell_content(cell)
        if result in words:
            return result
    return

def info_is_valid_path(board: Board, path: Path, words: Iterable[str]) -> Optional[str]:
    """in addition to _is_pattern_valid checks, checkes if the the word is legal,
    if invalid returns an str decricing the problem, else - None"""
    result = ""
    boogle_board = BoggleBoard(board)
    if not _is_pattern_valid(boogle_board, path):
        return(False, "invalid pattern <3")
    else:
        for cell in path:
            result += boogle_board.cell_content(cell)
        if result in words:
            return (True, result)
        return(False, "word not in dictionary :)")

#######################################find_n_paths##########################################

def _n_paths_helper(n: int, board: BoggleBoard, words: Iterable[str], partial_words: List[str], paths: List[Path], prev_cells: Path, cur_cell: Tuple[int], cur_word: str):
    """recursive function to finf all n long legal paths in board, returns list of legal paths"""
    if len(prev_cells) == n:
        if cur_word in words:
            paths.append(copy.deepcopy(prev_cells))
        return paths
    for next_cell in squere(cur_cell):
        if next_cell in board.cell_list() and next_cell not in prev_cells:
            prev_cells.append(next_cell)
            prev_word = cur_word
            cur_word += board.cell_content(next_cell)
            # if cur_word in partial_words:
            paths = _n_paths_helper(
                n, board, words, partial_words, paths, prev_cells, next_cell, cur_word)
            prev_cells.pop()
            cur_word = prev_word
    return paths

def find_length_n_paths(n: int, board: Board, words: Iterable[str]) -> List[Path]:
    """finf all n long legal paths in board, returns list of legal paths"""
    if n == 0:
        return []
    if n > len(board)*len(board[0]):
        return []
    if not board:
        return
    if not board[0]:
        return
    partial_words = None
    paths = []
    boogle = BoggleBoard(board)
    for cell in boogle.cell_list():
        cells_paths = _n_paths_helper(
            n, boogle, words, partial_words, [], [cell], cell, boogle.cell_content(cell))
        if cells_paths:
            for path in cells_paths:
                paths.append(path)
    return paths

##################################find_length_n_words########################################

def _n_words_helper(n: int, board: BoggleBoard, words: Iterable[str], paths: List[Path], prev_cells: Path, cur_cell: Tuple[int], cur_word: str):
    """recursive function to finf all n long legal words in board, returns list of legal paths"""
    if len(cur_word) >= n:
        if len(cur_word) == n and cur_word in words:
            paths.append(copy.deepcopy(prev_cells))
        return paths
    for next_cell in squere(cur_cell):
        if next_cell in board.cell_list() and next_cell not in prev_cells:
            prev_cells.append(next_cell)
            prev_word = cur_word
            cur_word += board.cell_content(next_cell)
            # if cur_word in partial_words:
            paths = _n_words_helper(
                n, board, words, paths, prev_cells, next_cell, cur_word)
            prev_cells.pop()
            cur_word = prev_word
    return paths

def find_length_n_words(n: int, board: Board, words: Iterable[str]) -> List[Path]:
    """finf all n long legal words in board, returns list of legal paths"""
    if n == 0:
        return []
    if n > len(board)*len(board[0]):
        return []
    if not board:
        return []
    if not board[0]:
        return []
    paths = []
    boogle = BoggleBoard(board)
    for cell in boogle.cell_list():
        cells_paths = _n_words_helper(
            n, boogle, words, [], [cell], cell, boogle.cell_content(cell))
        if cells_paths:
            for path in cells_paths:
                paths.append(path)
    return paths

##############################max_score######################################
        
def max_score_paths(board: Board, words: Iterable[str]) -> List[Path]:
    """finds the best list og paths to use, to get maximum score, returns the list"""
    optimal_pathes = {}
    optional_words = set(word for word in words)
    def _max_helper(b_board: BoggleBoard, words: Iterable[str], path: Path, first_cell: Tuple[int]):
        cur_word = b_board.paths_word(path)
        nonlocal optional_words
        nonlocal optimal_pathes
        prev_optional_words = set(word for word in optional_words)
        optional_words = set(word for word in optional_words if word.startswith(cur_word))
        # if cur_word in optional_words:
        if cur_word in words:
            if cur_word not in optimal_pathes:
                optimal_pathes[cur_word] = path[:]
            elif len(optimal_pathes[cur_word]) < len(path):
                optimal_pathes[cur_word] = path[:]
        for next_cell in squere(first_cell):
            if next_cell not in path and next_cell in b_board.cell_list():
                prev_path = copy.deepcopy(path)
                path.append(next_cell)
                if set(word for word in optional_words if word.startswith(b_board.paths_word(path))):
                    _max_helper(b_board, words, path, next_cell)
                path = prev_path
        optional_words = prev_optional_words
        return
    b_board = BoggleBoard(board)
    for i in range(len(board)):
        for j in range(len(board[0])):
            _max_helper(b_board, words, [(i,j)], (i,j))
    return [path for path in optimal_pathes.values()]
