#####################################################importing#####################################################

from typing import Optional
from game_display import GameDisplay
from typing import Tuple, List, Dict, Optional, Union
from game_utils import get_random_apple_data, get_random_wall_data


#####################################################constants#####################################################

UP = "Up"
DOWN = "Down"
RIGHT = "Right"
LEFT = "Left"



#####################################################class Snake#####################################################

class Snake:
    """Creates a new snake object and initializes it"""
    def __init__(self, x: int, y: int, length: int, direction: str) -> None:
        self.__head: Tuple[int] = (x, y)
        self.__length: int = length
        self.__direction: str = direction
        # initializing the locations according to the snake direction
        self.__locations: str = ""
        if direction == "Up":
            self.__locations = [(x, y-i) for i in range(length)]

        if direction == "Down":
            self.__locations = [(x, y+i) for i in range(length)]

        elif direction == "Right":
            self.__locations = [(x-i, y) for i in range(length)]

        elif direction == "Left":
            self.__locations = [(x+i, y) for i in range(length)]

    def snake_cells(self) -> List[Tuple[int]]:
        "This function return the location of the snake"
        return self.__locations

    def move_snake(self, move_key: str, extending: bool) -> bool:
        "This function move the snake to a new location, return true if succeeded"
        head = self.__locations[0]
        new_cell = None

        # change direction and determine new head location according to move_key
        if move_key == UP:
            new_cell = (head[0], head[1] + 1)
            self.__direction = UP

        elif move_key == DOWN:
            new_cell = (head[0], head[1] - 1)
            self.__direction = DOWN

        elif move_key == RIGHT:
            new_cell = (head[0] + 1, head[1])
            self.__direction = RIGHT

        elif move_key == LEFT:
            new_cell = (head[0]-1, head[1])
            self.__direction = LEFT

        # if move_key is legal move head to the new location
        if new_cell:
            self.__locations.insert(0, new_cell)
            if extending:
                return True
            self.__locations.pop()  # remove tail
            return True
        return False

    def split_snake(self, loc: Tuple[int]) -> None:
        "This function split the snake in case of wall hitting him"
        ind = self.__locations.index(loc)
        self.__locations = self.__locations[:ind]

    def get_direction(self) -> str:
        "This function returns the direction the snake at"
        return self.__direction

    def change_direction(self, direction: str) -> None:
        "This function changes the direction of the snake"
        self.__direction = direction


#####################################################class Apple#####################################################


class Apple:
    def __init__(self, location: Tuple[int]) -> None:
        """Creates a new Apple object and initializes it"""
        self.__location: Tuple[int] = location

    def get_location(self) -> Tuple[int]:
        "This function returns the location of the apple"
        return self.__location



#####################################################class Wall#####################################################

class Wall:

    def __init__(self, location: tuple, direction: str) -> None:
        """Creates a new Wall object and initializes it"""
        self.__location: Tuple[int] = location
        self.__direction: str = direction

    def get_cells(self) -> List[Tuple[int]]:
        "This function return the cells in board that the wall at"
        if self.__direction == "Up" or self.__direction == "Down":
            return [(self.__location[0], self.__location[1]+i) for i in range(-1, 2)]

        elif self.__direction == "Left" or self.__direction == "Right":
            return [(self.__location[0]+i, self.__location[1]) for i in range(-1, 2)]

    def move(self) -> None:
        "This function move the wall to a new loction according to its direction"
        if self.__direction == UP:
            self.__location = (self.__location[0], self.__location[1]+1)

        elif self.__direction == DOWN:
            self.__location = (self.__location[0], self.__location[1]-1)

        elif self.__direction == RIGHT:
            self.__location = (self.__location[0]+1, self.__location[1])

        elif self.__direction == LEFT:
            self.__location = (self.__location[0]-1, self.__location[1])

#####################################################class Board#####################################################


class Board:
    def __init__(self, height: int, width: int) -> None:
        """Creates a new Board object and initializes it"""
        self.__height: int = height
        self.__width: int = width
        self.__snake: Snake = None
        self.__apples: dict[Apple: Tuple[int]] = {}
        self.__walls: dict[Wall: Tuple[int]] = {}

    def cell_list(self) -> List[Optional[Tuple[int]]]:
        """returns a list of all the cells in the board"""
        lst = []
        for i in range(self.__width):
            for j in range(self.__height):
                lst.append((i, j))

        return sorted(lst)

    def cell_content(self, loc: Tuple[int]) -> List[Union[Snake, Apple, Wall, None]]:
        """returns the content of a given cell in the board"""
        
        list_object = []

        if self.__snake and loc in self.__snake.snake_cells():
            list_object.append(self.__snake)

        for apple in self.__apples:
            if loc == apple.get_location():
                list_object.append(apple)
    
        for wall in self.__walls:
            if loc in wall.get_cells():
                list_object.append(wall)

        if len(list_object) == 0:
            return None

        return list_object

    def add_snake(self, snake: Snake) -> bool:
        """ads a given snake to the board, returns True if succesed, else - False"""

        for cell in snake.snake_cells():
            if cell not in self.cell_list():
                return False

        self.__snake = snake
        return True

    def add_apple(self, apple: Apple) -> bool:
        """ads a given apple to the board, returns True if succesed, else - False"""

        if self.__snake and apple.get_location() == self.__snake.snake_cells()[0]:
            self.__apples[apple] = apple.get_location()
            return True

        if apple.get_location() not in self.cell_list() or self.cell_content(apple.get_location()):
            return False

        self.__apples[apple] = apple.get_location()
        return True

    def add_wall(self, wall: Wall) -> bool:

        """ads a given wall to the board, returns True if succesed, else - False"""

        if wall.get_cells()[1] in self.cell_list():
            for cell in wall.get_cells():
                if self.cell_content(cell):
                    return False

            self.__walls[wall] = wall.get_cells()
            return True
    
        return False

    def get_snake(self) -> Snake:
        """returns the games snake"""
        return self.__snake

    def get_walls(self):
        """returns a dict that maps each wall in the board to it's list of cells"""
        return self.__walls

    def get_apples(self):
        """returns a dict that maps each apple in the board to it's location"""
        return self.__apples

    def remove_apple(self, apple: Apple) -> None:
        """removes a given apple off the board"""
        self.__apples.pop(apple)

    def remove_wall(self, wall: Wall) -> None:
        """removes a given wall off the board"""
        self.__walls.pop(wall)



#####################################################class SnakeGame#####################################################

class SnakeGame:

    def __init__(self, board, apples_amount: int, walls_amount: int, debug_mode, iterations: int = -1) -> None:
        """creates a new SnakeGame object and initializes it"""
        self.__debug_mode: bool = debug_mode
        self.__board: Board = board
        self.__key_clicked: Optional[str] = None
        self.__stop_iteration: int = iterations
        self.__cur_iteration: int = 0
        self.__apples_amount: int = apples_amount
        self.__walls_amount: int = walls_amount
        self.__extending: int = 0
        self.__score: int = 0
        self.__apple_was_eaten: bool = False
        self.__wall_was_removed: bool = False
        self.__apple_was_removed: int = False
        self.__snake_spliting: Optional[bool] = None

    def read_key(self, key_clicked: Optional[str]) -> None:
        """ recives the key clicked and saves it into a class var"""
        self.__key_clicked = key_clicked

    def __check_key_clicked(self) -> bool:
        """checkes is the key that was clicked is legal:
        a snake facing up or down can only be directed left or right
        a snake facing right or left can only be directed up or down"""

        if not self.__key_clicked:
            return False

        #check if the key clicked fits for the snake's direction
        cur_direction = self.__board.get_snake().get_direction()
        if self.__key_clicked == UP or self.__key_clicked == DOWN:
            if cur_direction == RIGHT or cur_direction == LEFT:
                return True

        if self.__key_clicked == RIGHT or self.__key_clicked == LEFT:
            if cur_direction == UP or cur_direction == DOWN:
                return True
    
        return False

    def __move_snake_straight(self) -> None:
        """moves the snake the same direction it's facing"""

        direction = self.__board.get_snake().get_direction()
        if direction == UP:
            self.__board.get_snake().move_snake(UP, self.__extending)

        elif direction == DOWN:
            self.__board.get_snake().move_snake(DOWN, self.__extending)

        elif direction == RIGHT:
            self.__board.get_snake().move_snake(RIGHT, self.__extending)

        elif direction == LEFT:
            self.__board.get_snake().move_snake(LEFT, self.__extending)

    def __is_snake_splited(self) -> Optional[Tuple[int]]:
        """checks if the snake got splited by a wall - that means the
        a wall cut the snake on it's third cell or further
        if the snake is splited = return the closest to the snake's head cell that is cut
        otherwise returns None"""

        snake_cells = self.__board.get_snake().snake_cells()
        for i in range(2, len(snake_cells)):
                content = self.__board.cell_content(snake_cells[i])
                for item in content:
                    if isinstance(item, Wall):
                        return snake_cells[i]
        return


    def __move_snake(self) -> None:
        """moves the snake as it should be moving on current round"""

        #split, if snake should split
        split_cell = self.__snake_spliting
        if split_cell:
            self.__board.get_snake().split_snake(split_cell)

        # turn, if a legal key was clicked
        if self.__check_key_clicked():
            if self.__key_clicked == UP:
                self.__board.get_snake().move_snake(UP, self.__extending)
            elif self.__key_clicked == DOWN:
                self.__board.get_snake().move_snake(DOWN, self.__extending)
            elif self.__key_clicked == RIGHT:
                self.__board.get_snake().move_snake(RIGHT, self.__extending)
            elif self.__key_clicked == LEFT:
                self.__board.get_snake().move_snake(LEFT, self.__extending)

        # if not turning, keep moving on same direction
        else:
            self.__move_snake_straight()

        # if the snake was extending in this turn, now, after moving and extending it, wer'e updating extending var
        if self.__extending:
            self.__extending -= 1

    def __move_walls(self) -> None:
        """move existing walls and remove walls that left the board from walls dict"""

        walls_to_remove = []
        #moving all the walls and checking wich ones got out of the board
        for wall in self.__board.get_walls():
            if self.__cur_iteration % 2 == 0:
                wall.move()
                wall_in_board = False
                for loc in wall.get_cells():
                    if loc in self.__board.cell_list():
                        wall_in_board = True
                if not wall_in_board:
                    walls_to_remove.append(wall)

        #removing walls that left the board
        for wall in walls_to_remove:
            self.__wall_was_removed = True
            self.__board.remove_wall(wall)
        
        #determine and updates weather the snake got splitted by one of the walls
        if not self.__debug_mode:
            self.__snake_spliting = self.__is_snake_splited()

    def __did_snake_eat(self) -> None:
        """determine if the snake is eating an apple, if so - update extanding var"""
        for apple in self.__board.get_apples():
            if apple.get_location() == self.__board.get_snake().snake_cells()[0]:
                self.__apple_was_eaten = True
                self.__extending += 3
    
    def __remove_aplles(self) -> None:
        """remove apples that were destroyed by walls or eaten by the snake
        update fitting class var if an apple should be removed"""

        #looking for apples that should be removed
        apples_to_remove = set()
        for apple in self.__board.get_apples():
            for content in self.__board.cell_content(apple.get_location()):
                if not isinstance(content, Apple):
                    apples_to_remove.add(apple)
        
        #removing
        for apple in apples_to_remove:
            self.__board.remove_apple(apple)
            self.__apple_was_removed = True


    def update_objects(self) -> Tuple[bool, int, bool]:
        """update all objects on board
        returns the number of walls we removed, the number apples we removed,
        and bool for wheather the snake ate an apple on this round"""

        # update the snake
        if not self.__debug_mode:
            self.__move_snake()

        # update walls
        self.__move_walls()

        # update apples
        # determine if the snake is eating an apple, if so - update extanding var
        if not self.__debug_mode:
            self.__did_snake_eat()

        # remove apples that were destroyed by walls
        self.__remove_aplles()
        return self.__wall_was_removed, self.__apple_was_removed, self.__apple_was_eaten



    def add_objects(self, removed_walls: bool, removed_apples: bool) -> None:
        """add required objects to the board"""

        #add walls, if needed
        if len(self.__board.get_walls()) < self.__walls_amount:
            wall_data = get_random_wall_data()
            wall = Wall((wall_data[0], wall_data[1]), wall_data[2])
            self.__board.add_wall(wall)

        #if no apples was removed, try to add one, if needed
        if len(self.__board.get_apples()) < self.__apples_amount:
            apple_data = get_random_apple_data()
            apple = Apple(apple_data)
            self.__board.add_apple(apple)


    def draw_board(self, gd: GameDisplay):
        """draws the board"""

        #draw the apples
        for apple in self.__board.get_apples():
            cell = apple.get_location()
            gd.draw_cell(cell[0], cell[1], "green")
        
        #draw the snake, run over apples if needed
        if not self.__debug_mode:
            for cell in self.__board.get_snake().snake_cells():
                if cell in self.__board.cell_list():
                    gd.draw_cell(cell[0], cell[1], "black")

        #draw the walls, run over apples and the snake if needed
        for wall in self.__board.get_walls():
            for cell in wall.get_cells():
                if cell in self.__board.cell_list():
                    gd.draw_cell(cell[0], cell[1], "blue")


    def end_round(self) -> None:
        """prepare the class-vars for the next round:
        prociding to next iteration, updating that if nothing happens- no key was clicked,
        no apples yet were eaten, no walls yet removed and no apples yet have been removed"""
        self.__cur_iteration += 1
        self.__key_clicked = None
        self.__apple_was_eaten = False
        self.__wall_was_removed = False
        self.__apple_was_removed = False

    def is_over(self) -> bool:
        """checks if the game sould be over, if so return True, else - False"""

        #if we passed the number of allowed iteraions, stop the game
        if self.__cur_iteration > self.__stop_iteration and self.__stop_iteration != -1:
            return True
        
        #if debug mode, and iteration num is allowed, no reson to stop the game
        if self.__debug_mode:
            return False
        
        snake_cells = self.__board.get_snake().snake_cells()
        #if snakes head or one cell next to it was hitten by wall, game is over
        for i in range(2):
            if len(snake_cells) >= i+1:
                for item in self.__board.cell_content(snake_cells[i]):
                    if isinstance(item, Wall):
                        return True
        
        #if snake crashed into board's border, game is over
        for cell in snake_cells:
            if cell not in self.__board.cell_list():
                return True

        #if snake crushed into itself, game is over
        if len(snake_cells) != len(set(snake_cells)):
            return True
        return False
    
    def get_board(self) -> Board:
        """returns the game's board"""
        return self.__board
    
    def update_score(self,previous_length) -> None:
        """updates the score according to the given snake length"""
        self.__score += int(previous_length**0.5)

    def get_score(self) -> int:
        """returns the current game score"""
        return self.__score
