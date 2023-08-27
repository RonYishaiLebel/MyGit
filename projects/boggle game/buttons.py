#########################importing#########################

import tkinter as tk
from typing import Tuple, List, Dict

##########################constants############################

Font = Tuple[str, int]

############################LetterButton#########################

class LetterButton:
    def __init__(self, root, bg: str, text: str, font: Font, width: int, height: int, loc: Tuple[int], cur_word: tk.Label, clicked_buttons: Dict[tk.Button, Tuple[int]]) -> None:
        self.__button = tk.Button(root, bg=bg, text=text, font=font, width=width,
                                  height=height, command=self.__click_button)
        self.__button.bind('<Enter>', self.__on_enter)
        self.__button.bind('<Leave>', self.__on_leave)
        self.__clicked = False
        self.__location = loc
        self.__clicked_buttons = clicked_buttons
        self.__cur_word = cur_word

    def __on_enter(self, e):
        """change color of the button when mouse hover the button"""
        if e.widget['bg'] == 'white':
            e.widget['bg'] = 'azure3'
        if e.widget['bg'] == 'violet':
            e.widget['bg'] = 'LightPink1'

    def __on_leave(self, e):
        """change color of the button when mouse stop hovering the button"""
        if self.__clicked:
            color = 'violet'
        else:
            color = 'white'
        e.widget['bg'] = color

    def get_button(self) -> tk.Button:
        """return a button object"""
        return self.__button

    def is_button_clicked(self) -> bool:
        """return true if button is clicked"""
        return self.__clicked

    def __adapt_color(self) -> None:
        """change color according to current color"""
        if self.__clicked:
            self.__button['bg'] = 'violet'
        else:
            self.__button['bg'] = 'white'

    def __update_clicked_buttons(self) -> None:
        """update the current status of the buttons"""
        if self.__clicked:
            self.__clicked_buttons[self.__button] = self.__location
        else:
            if self.__button in self.__clicked_buttons:
                self.__clicked_buttons.pop(self.__button)

    def __update_cur_word(self) -> None:
        """update the current combination"""
        new_word = ""
        for button in self.__clicked_buttons:
            new_word += button['text']
        self.__cur_word.config(text=new_word)
        if self.__cur_word['text']:
            self.__cur_word.grid()
        else:
            self.__cur_word.grid_remove()

    def __click_button(self) -> None:
        """call all the function that are called when click on a button"""
        self.__clicked = not self.__clicked
        self.__adapt_color()
        self.__update_clicked_buttons()
        self.__update_cur_word()

    def unlock(self) -> None:
        """turn the button to be unclicked"""
        if self.__clicked:
            self.__clicked = False
            self.__adapt_color()
            self.__update_cur_word()
            self.__update_clicked_buttons()

    def get_location(self) -> Tuple[int]:
        """return the location of the button"""
        return self.__location
