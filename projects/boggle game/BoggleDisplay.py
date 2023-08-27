############################################importing####################################

import tkinter as tk
from tkinter import messagebox
import BoggleGame as bg
from BoggleBoard import BoggleBoard
import pygame
from timer import Timer
from score import Score
from used_words import UsedWords
from buttons import LetterButton
from typing import List, Dict, Tuple
import sys

#########################################constants#######################################

TIME = 180
FONT = ("Arial", 14)
LBUTTON_WIDTH = 20
LBUTTON_HEIGHT = 2
OPENING_LRTTER: str = "Welcome to Boggle!\n\n"\
    "Your mission is to to find as many words as you can, in 3 minutes.\n"\
    "You can start wherever you want on the board, and move to each one of "\
    "the cells cercling your current cell, but you can't use the same "\
    "cell twice for the same word.\n"\
    "Also, you can't get the same word twice.\n"\
    "Notice! Your score depends on the length of the path, not the length of the word!\n\n"\
    "ENJOY! :)"

##########################################class ColorBoard##################################

class ColorBoard:
    def __init__(self, game: bg.BoggleGame):
        # init window
        self.__root = tk.Tk()
        self.__boggle_game: bg.BoggleGame = game
        self.__root.wm_iconphoto(False, tk.PhotoImage(file='BG_logo.png'))
        self.__root.title('Boggle Game')
        self.__root.resizable(False, False)
        self.__root.geometry("900x450+200+200")
        if sys.platform == "linux":
            self.__root.bind(
                '<Configure>', lambda event: self.maintain_aspect_ratio(event, 900/450))

        # init background image
        self.__bgimage = tk.PhotoImage(file='Boggle_background.png')
        self.__bg_label = tk.Label(self.__root, image=self.__bgimage)
        self.__bg_label.place(x=0, y=0)

        # current combination label
        self.__cur_word = tk.Label(self.__root, text="",
                                 relief=tk.FLAT, font=FONT)
        self.__cur_word.grid(row=20, column=1, columnspan=4)
        self.__cur_word.grid_remove()

        # init buttons
        self.__buttons: List[LetterButton] = []
        self.__clicked_buttons: Dict[tk.Button, Tuple[int]] = {}
        self.__init_buttons()

        # timer label
        self.__timer = None
        self.__timer_label = None
        self.__init_timer()

        # score label
        self.__score_label = None
        self.__init_score()

        # start button
        self.__start_button = tk.Button(
            self.__root, text='Start', command=self.start_game, font=("Arial", 14), width=5, anchor="center")
        self.__start_button.grid(row=6, column=0, columnspan=4)
        self.__root.grid_columnconfigure(0, weight=3)
        self.__root.grid_rowconfigure(6, weight=3)

        # Input word button
        self.__input_button = tk.Button(
            self.__root, text='Input Word', command=self.check_word, font=("Arial", 14))
        self.__input_button.grid(row=6, column=1, columnspan=4)
        self.__input_button.grid_remove()

        # found word label
        self.__found_word_label = None
        self.__init_used_words()

        # welcome message
        tk.messagebox.showinfo(title="Welcome", message=OPENING_LRTTER)

    def __init_timer(self):
        """init the timer label"""
        self.__timer = TIME
        self.__timer_label = Timer(self.__root, TIME, FONT, "white", 5, 2)
        self.__timer_label.get_timer().grid(row=0, column=2, columnspan=2)
        self.__timer_label.get_timer().grid_remove()

    def __init_buttons(self):
        """init letter buttons"""
        for i in range(len(self.__boggle_game.get_board().get_board())):
            for j in range(len(self.__boggle_game.get_board().get_board()[i])):
                button = LetterButton(self.__root, "white", self.__boggle_game.get_board().cell_content(
                    (i, j)), FONT, LBUTTON_WIDTH, LBUTTON_HEIGHT, (i, j), self.__cur_word, self.__clicked_buttons)
                button.get_button().grid(row=i+1, column=j+1)
                button.get_button().grid_remove()
                self.__buttons.append(button)

    def __init_score(self) -> None:
        """init the score label"""
        self.__score_label = Score(self.__root, FONT)
        self.__score_label.get_score().grid(row=0, column=0, columnspan=4, sticky='w')
        self.__score_label.get_score().grid_remove()

    def __init_used_words(self) -> None:
        """init the label of the words that were already found"""
        self.__found_word_label = UsedWords(
            self.__root, "Found words:", FONT, 'white')
        self.__found_word_label.get_used_words().grid(row=22, column=1, columnspan=4)
        self.__found_word_label.get_used_words().grid_remove()

    def maintain_aspect_ratio(self, event, aspect_ratio):
        """ Event handler to override root window resize events to maintain the
            specified width to height aspect ratio.
        """
        if event.widget.master:  # Not root window?
            return  # Ignore.

        # <Configure> events contain the widget's new width and height in pixels.
        new_aspect_ratio = event.width / event.height

        # Decide which dimension controls.
        if new_aspect_ratio > aspect_ratio:
            # Use width as the controlling dimension.
            desired_width = event.width
            desired_height = int(event.width / aspect_ratio)
        else:
            # Use height as the controlling dimension.
            desired_height = event.height
            desired_width = int(event.height * aspect_ratio)

        # Override if necessary.
        if event.width != desired_width or event.height != desired_height:
            # Manually give it the proper dimensions.
            event.widget.geometry(f'{desired_width}x{desired_height}')
            return "break"  # Block further processing of this event.

    def play_music(self):
        """This function responsable on plaing rhe music"""
        pygame.mixer.init()
        pygame.mixer.music.load("Boggle_sound.mp3")
        pygame.mixer.music.play(-1)

    def __set_new_board(self):
        """starts a new board"""
        self.__boggle_game.set_new_board()
        self.__init_buttons()

    def __restart_buttons(self) -> None:
        """restart the letter buttons"""
        for button in self.__buttons:
            button.get_button().grid()
        self.__start_button.grid_remove()

    def __start_components(self) -> None:
        """show the grid of all labels except word buttons"""
        self.__timer_label.get_timer().grid()
        self.__input_button.grid()
        self.__found_word_label.get_used_words().grid()
        self.__score_label.get_score().grid()

    def start_game(self):
        """start the game after clicking on start button"""
        # start game after pushing start button
        self.__restart_buttons()

        # show all parts of dispaly that were hidden
        self.__start_components()

        # start timer
        self.__root.after(1000, self._decrement_timer)

    def _timer_text(self):
        """ This function calculate what the timer should show according to seconds left"""
        if self.__timer % 60 < 10:
            return '0' + str(self.__timer//60) + ':' + '0' + str(self.__timer % 60)
        else:
            return '0' + str(self.__timer//60) + ':' + str(self.__timer % 60)

    def __clear_after_input(self) -> None:
        for button in self.__buttons:
            button.unlock()
        self.__cur_word.grid_remove()

    def check_word(self):
        """ This function check if word vaild, shows error messege if not and update score and words
        used if is valid """
        Error_str = self.__boggle_game.pass_input(
            list(self.__clicked_buttons.values()))
        if Error_str:
            Error_msg = tk.Toplevel(self.__root)
            Error_msg.geometry("300x100+75+500")
            Error_msg.title("Error")
            self.error_label = tk.Label(
                Error_msg, text=Error_str, font='Arial 15')
            self.error_label.pack()
            self.error_label.after(2000, Error_msg.destroy)
        else:
            self.__found_word_label.config_found_words(
                self.__boggle_game.get_used_words())
            self.__score_label.config_score(str(self.__boggle_game.get_score()))

        # clear what was chosen before input
        self.__clear_after_input()

    def _decrement_timer(self):
        """ This function control the updating of timer according to time"""
        time_is_over = self.__timer_label.decrement_timer()
        if time_is_over:
            self.end_game()
        else:
            self.__root.after(1000, self._decrement_timer)

    def __reset_board_and_butoons(self) -> None:
        """reset the board and buttons to their original status"""
        self.__set_new_board()
        self.__boggle_game.reset_score()
        for button in self.__buttons:
            button.get_button().config(bg="white")
            button.unlock()
        self.__cur_word.config(text="")
        self.__cur_word.grid_remove()

    def __reset_timer(self) -> None:
        """restart the timer with the original time"""
        self.__timer = TIME
        self.__timer_label.get_timer().grid_remove()
        self.__init_timer()

    def __reset_in_Boggle_game(self) -> None:
        """restart the used word, score and the input button"""
        self.__found_word_label.get_used_words().config(text="Found words: ")
        self.__score_label.config_score("")
        self.__boggle_game.reset_used_words()
        self.__input_button.grid_remove()

    def end_game(self):
        """ This function check if game ended and player want to play again
        reset all labels to init config """
        self.__root.after_cancel(self._decrement_timer)
        result = tk.messagebox.askyesno(
            "Time's up!", "Do you want to play again?")
        if result:
            # return all objects to the state of game start
            self.__reset_board_and_butoons()
            self.__reset_timer()
            self.__reset_in_Boggle_game()
            self.start_game()
        else:
            self.__root.destroy()

    def run(self):
        """"This function run the display"""
        self.play_music()
        self.__root.mainloop()


if __name__ == '__main__':
    pass
