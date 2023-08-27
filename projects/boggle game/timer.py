import tkinter as tk
from typing import Tuple, List


class Timer:

    def __init__(self, root, time: int, font: Tuple[str, int], background: str, width: int, height: int) -> None:
        self.__time = time
        self.__timer_text = self.__show_time()
        self.__timer_label = tk.Label(
            root, text=self.__timer_text, font=font, bg=background, width=width, height=height)

    def __show_time(self) -> str:
        """ This function calculate what the timer should show according to seconds left"""
        if self.__time % 60 < 10:
            return '0' + str(self.__time//60) + ':' + '0' + str(self.__time % 60)
        else:
            return '0' + str(self.__time//60) + ':' + str(self.__time % 60)

    def decrement_timer(self) -> bool:
        """ This function control the updating of timer according to time"""
        time_left = self.__time - 1
        if time_left >= 0:
            self.__time = time_left
            self.__timer_label.config(text=self.__show_time())
            return False
        else:
            return True

    def get_timer(self) -> tk.Label:
        """return current time"""
        return self.__timer_label
