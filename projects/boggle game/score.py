#########################IMPORTING#######################
import tkinter as tk
from typing import List, Tuple

#########################COSNTANTS########################

SCORE = "Score: "

#########################SCORE CLASS######################

class Score:
    def __init__(self, root, font: Tuple[str, int]) -> None:
        self.__score = 0
        self.__score_label = tk.Label(
            root, text=SCORE, font=font)

    def get_score(self) -> tk.Label:
        """return the score"""
        return self.__score_label

    def config_score(self, new_score: str) -> None:
        """change the score"""
        self.__score_label.config(
            text='Score: ' + new_score)
