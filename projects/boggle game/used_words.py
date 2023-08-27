import tkinter as tk
from typing import List, Tuple


class UsedWords:
    def __init__(self, root, text: str, font: Tuple[str, int], bg: str) -> None:
        self.__used_words_label = tk.Label(root, text=text, font=font, bg=bg)

    def get_used_words(self) -> tk.Label:
        """return the found words"""
        return self.__used_words_label

    def config_found_words(self, used_words: str) -> None:
        """change the found words"""
        self.__used_words_label.config(text='Found words: ' + used_words)
