##################################################### importing #####################################################
import argparse
import game_utils
from snake_game import SnakeGame, Board, Snake, Apple, Wall
from game_display import GameDisplay


##################################################### main functions #####################################################

def single_turn(gd: GameDisplay, game: SnakeGame, debug_mode: bool):
    "This function runs a single turn of snake game"
    # get info on click from user
    key_clicked = gd.get_key_clicked()
    game.read_key(key_clicked)

    # update objects
    if not debug_mode:
        prev_length = len(game.get_board().get_snake().snake_cells())
    wall_was_removed, apple_was_removed, apple_was_eaten = game.update_objects()

    # if an apple was eaten update score
    if not debug_mode and apple_was_eaten:
        game.update_score(prev_length)
        gd.show_score(game.get_score())

    # add objects according to the number of objects removed from board during the turn
    game.add_objects(wall_was_removed, apple_was_removed)


def main_loop(gd: GameDisplay, args: argparse.Namespace) -> None:
    """main loop of a snake game"""
    # create board
    board = Board(args.height, args.width)

    # create snake if needed
    if not args.debug:
        snake = Snake(args.width//2, args.height//2, 3, "Up")
        board.add_snake(snake)

    # create wall if needed
    if args.walls:
        wall_data = game_utils.get_random_wall_data()
        wall = Wall((wall_data[0], wall_data[1]), wall_data[2])
        board.add_wall(wall)

    # create apple if needed
    if args.apples:
        apple_data = game_utils.get_random_apple_data()
        apple = Apple(apple_data)
        board.add_apple(apple)

    # create game and draw it
    game = SnakeGame(board, args.apples, args.walls, args.debug, args.rounds)
    gd.show_score(0)
    game.draw_board(gd)

    # prepare for next round
    game.end_round()
    gd.end_round()
    # end of round 0

    # run game as long it is not over
    while not game.is_over():
        single_turn(gd, game, args.debug)
        game.draw_board(gd)
        game.end_round()
        gd.end_round()


##################################################### main #####################################################

if __name__ == "__main__":
    print("You should run:\n"
          "> python game_display.py")
