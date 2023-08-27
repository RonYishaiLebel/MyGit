#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60
#define VALID_ARGC 3
#define DECIMAL 10
#define SEED_IND 1
#define ROUTES_IND 2
#define USAGE "Usage: The program require two params\n"

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20

/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of the ladder in case
    // there is one from this square
    int snake_to;  // snake_to represents the jump of the snake in case
    // there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_database(database);
    }
    return EXIT_FAILURE;
}

/**
 * creates a new board of the game
 * @param cells an array of BOARD_SIZE pointers to Cell structs.
 * @return 0 upon success, ekse - 1.
 */
static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node= get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database
                (markov_chain, cells[index_to])->data;
            add_node_to_frequencies_list
            (from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database
                    (markov_chain, cells[index_to])->data;
                add_node_to_frequencies_list
                (from_node, to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

/**
 * check if argc is valid
 * @param argc argc
 * @return true for valid, falsr for invalid.
 */
static bool check_argc(int argc)
{
  if (argc != VALID_ARGC)
  {
    return false;
  }
  return true;
}


/**
 * create a new database
 * @return a new database - pointer to LinkedList
 */
static LinkedList *create_database ()
{
  LinkedList *database = calloc (1, sizeof (*database));
  if (!database)
  {
    return NULL;
  }
  database->first = NULL;
  database->last = NULL;
  database->size = 0;
  return database;
}


/**
 * print finction for snakes_and_ladders
 * @param data it will be converted to pointer to Cell.
 */
static void print_func (void *data)
{
  Cell * conv_data = (Cell *) data;
  printf ("[%d]", conv_data->number);
  if (conv_data->ladder_to != -1)
  {
    printf ("-ladder to %d -> ", conv_data->ladder_to);
  }
  else if (conv_data->snake_to != -1)
  {
    printf("-snake to %d -> ", conv_data->snake_to);
  }
  else if (conv_data->number != BOARD_SIZE)
  {
    printf (" -> ");
  }
}


/**
 * free func for the game
 * @param data will be converted to pointer to Cell
 */
static void free_func (void *data)
{
  free ((Cell *) data);
}


/**
 * allocate and create a copy of the Cell delivered as void* data.
 * @param data will be converted to pointer to Cell and copied.
 * @return pointer to newly allocated Cell but as void*.
 */
static void *copy_func (void *data)
{
  Cell *conv_data = (Cell *) data;
  Cell *copy = calloc (1, sizeof (*copy));
  if (!copy)
  {
    return NULL;
  }
  copy->snake_to = conv_data->snake_to;
  copy->ladder_to = conv_data->ladder_to;
  copy->number = conv_data->number;
  return copy;
}


/**
 * check if cell is last in board or after the last one.
 * @param data will be converted to pointer to Cell.
 * @return true if last, false otherwise.
 */
static bool is_last (void *data)
{
  Cell *conv_data = (Cell *) data;
  if (conv_data->number < BOARD_SIZE)
  {
    return false;
  }
  return true;
}


/**
 * compare two Cell structs by there number field
 * @param data1 will be converted to pointer to Cell
 * @param data2 will be converted to pointer to Cell
 * @return negative value if data2>data1, postive if data1>data2, 0 if ==.
 */
static int comp_func (void *data1, void *data2)
{
  Cell * conv_data1 = data1, * conv_data2 = data2;
  return conv_data1->number - conv_data2->number;
}


/**
 * create a new markov chain.
 * @return a newly allocated markov chain
 */
static MarkovChain *create_markov_chain ()
{
  MarkovChain *markov_chain = calloc (1, sizeof (*markov_chain));
  if (!markov_chain)
  {
    return NULL;
  }
  LinkedList *database = create_database ();
  if (!database)
  {
    free (markov_chain);
    return NULL;
  }
  markov_chain->database = database;
  markov_chain->print_func = &print_func;
  markov_chain->comp_func = &comp_func;
  markov_chain->copy_func = &copy_func;
  markov_chain->free_data = &free_func;
  markov_chain->is_last = &is_last;
  return markov_chain;
}


/**
 * get num from str.
 * @param argv a string.
 * @return a num from the string.
 */
static int get_num (char *argv)
{
  int seed = 0;
  if (sscanf (argv, "%d", &seed) != EOF)
  {
    return seed;
  }
  return seed;
}





/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main (int argc, char *argv[])
{
  if (!check_argc (argc))
  {
    printf ("%s", USAGE);
    return EXIT_FAILURE;
  }
  MarkovChain * markov_chain = create_markov_chain();
  if (fill_database (markov_chain))
  {
    free_database (&markov_chain);
    printf ("%s", ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  int seed = get_num (argv[SEED_IND]);
  srand (seed);
  int routes_max = get_num (argv[ROUTES_IND]);
  for (int i =0; i< routes_max; i++)
  {
    printf ("Random Walk %d: ", i+1);
    generate_tweet (markov_chain, markov_chain->database->first->data,
                    MAX_GENERATION_LENGTH);
    printf ("\n");
  }
  free_database (&markov_chain);
  return EXIT_SUCCESS;
}