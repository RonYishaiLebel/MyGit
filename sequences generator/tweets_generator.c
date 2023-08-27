//
// Created by ron on 5/18/23.
//
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
#include "markov_chain.h"
#include "linked_list.h"
#define VALID_ARGC4 4
#define VALID_ARGC5 5
#define SPACE_CHAR ' '
#define DOT_CHAR '.'
#define THOUSAND 1001
#define HUNDRED 101
#define READ_MODE "r"
#define FOURTH 4
#define NO_LIMIT -1
#define USAGE "Usage: 3 or 4 params must be delivered\n"
#define DELIMITERS " \n\t\r"
#define MAX_TW_WORDS 20
#define DECIMAL 10

/**
 * get the number of words limit from argv
 * @param argv
 * @return int of words limit
 */
static int get_words_limit (char *argv[])
{
  int i = NO_LIMIT;
  sscanf (argv[FOURTH], "%d", &i);
  return i;
}


/**
 * cheack file opening.
 * @param file a file that we tried to open.
 * @return 0 upon failure, 1 for success.
 */
static int check_file_opening (FILE *file)
{
  if (file == NULL)
  {
    printf ("Error: couldn't open the file\n");
    return 0;
  }
  return 1;
}


/**
 * check of argc valid
 * @param argc
 * @return 0 if invalid, 1 if valid
 */
static int check_argc (int argc)
{
  if (argc != VALID_ARGC4 && argc != VALID_ARGC5)
  {
    printf ("%s", USAGE);
    return 0;
  }
  return 1;
}


/**
 * create a new database LinkedList
 * @return a new database LinkedList
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
 * free function for freeing char arrays in markov nodes.
 * @param data will be converted to char* as a word in markov node
 */
static void free_func (void *data)
{
  char *conv_data = (char *) data;
  free (conv_data);
}


/**
 * copy function for tweets - to make a new copy of a word in markov node
 * @param data will be converted to char* as a word in markov node
 * @return the new allocated copy of the word.
 */
static void *copy_func (void *data)
{
  char *conv_data = (char *) data;
  char *copy = calloc (strlen (conv_data)+1, sizeof (*copy));
  if (!copy)
  {
    return NULL;
  }
  strcpy (copy, conv_data);
  return copy;
}


/**
 * check if word is last - meaning ends with '.'
 * @param data will be converted to char* as a word in markov node
 * @return true if last, else - false.
 */
static bool is_last (void *data)
{
  char *conv_data = (char *) data;
  if (conv_data[strlen (conv_data) - 1] == DOT_CHAR)
  {
    return true;
  }
  return false;
}


/**
 * print function for tweets
 * @param data will be converted to char* as a word in markov node
 */
static void print_func (void *data)
{
  char *con_data = (char *) data;
  printf ("%s", con_data);
  if (!is_last (data))
  {
    printf (" ");
  }
}


/**
 * compare function comparing two markov nodes with strcmp on their data.
 * @param data1 will be converted to char* as a word in markov node
 * @param data2 will be converted to char* as a word in markov node
 * @return as in strcmp
 */
static int comp_func (void *data1, void *data2)
{
  char *conv_data1 = (char *) data1;
  char *conv_data2 = (char *) data2;
  return strcmp (conv_data1, conv_data2);
}

/**
 * create a new markov chain
 * @return a newly allocated markov chain, or NULL if failed to create.
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
 *read the tweets in fp, up to words_to_read, then store the data in
 * markov_chain.
 * @param fp a text file of tweets.
 * @param words_to_read maximum amount of words to read from the file.
 * @param markov_chain markov vhain for storing the data.
 * @return 0 upon success, else 1.
 */
static int fill_database(FILE *fp, int words_to_read, MarkovChain
*markov_chain)
{
  MarkovNode *prev_markov = NULL;
  int counter = 0;
  bool reached_limit = false;
  char cur_line[THOUSAND];
  while ((fgets (cur_line, THOUSAND, fp)) && !reached_limit)
  {
    char *cur_word = strtok (cur_line, DELIMITERS);
    while (cur_word != NULL && !reached_limit)
    {
      Node *cur_node = add_to_database (markov_chain, cur_word);
      if (!cur_node)
      {
        free_database (&markov_chain);
        return 1;
      }
      if (prev_markov)
      {
        if (!add_node_to_frequencies_list
        (prev_markov, cur_node->data, markov_chain))
        {
          free_database (&markov_chain);
          return 1;
        }
      }
      prev_markov = cur_node->data;
      if (cur_word[strlen (cur_word) - 1] == DOT_CHAR)
      {
        prev_markov = NULL;
      }
      counter += 1;
      if (words_to_read != -1 && counter >= words_to_read)
      {
        reached_limit = true;
      }
      cur_word = strtok (NULL, DELIMITERS);
    }
  }
  return 0;
}


/**
 * print tweets generated from the database
 * @param markov_chain markov chain storing tweets data
 * @param num_tweets number of tweets to be printed.
 */
static void print_tweets (MarkovChain *markov_chain, long num_tweets)
//untouched
{
  for (int i = 0; i < num_tweets; ++i)
  {
    printf ("Tweet %d: ", i + 1);
    MarkovNode *first_markov = get_first_random_node (markov_chain);
    generate_tweet (
        markov_chain, first_markov, MAX_TW_WORDS);
    printf ("\n");
  }
}


/**
 * get seed value delivered by the user in argv
 * @param argv argv
 * @return an int.
 */
static int get_seed (char *argv)
{
  int seed = 0;
  if (sscanf (argv, "%d", &seed) != EOF)
  {
    return seed;
  }
  return seed;
}


/**
 * read file of tweets, and by the information collected from it - print tweets
 * @param argc argc
 * @param argv supposed to be: 1. seed valur for randomizing
 * 2. amount of tweets to print. 3. path to tweets text file.
 * @return
 */
int main (int argc, char *argv[])
{
  if (!check_argc (argc))
  {
    return EXIT_FAILURE;
  }
  int seed = get_seed (argv[1]);
  srand (seed);
  int words_limit = NO_LIMIT;
  if (argc == VALID_ARGC5)
  {
    words_limit = get_words_limit (argv);
  }
  FILE *file = fopen (argv[3], READ_MODE);
  if (!check_file_opening (file))
  {
    return EXIT_FAILURE;
  }
  MarkovChain *markov_chain = create_markov_chain ();
  if (!markov_chain)
  {
    return 1;
  }
  if (fill_database (file, words_limit, markov_chain))
  {
    printf ("%s", ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  char *ptr;
  long num_tweets = strtol (argv[2], &ptr, DECIMAL);
  print_tweets (markov_chain, num_tweets);
  free_database (&markov_chain);
  fclose (file);
  return EXIT_SUCCESS;
}