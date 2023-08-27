//
// Created by ron on 5/18/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "markov_chain.h"
#define DOT_CHAR '.'
#define DOT_STR "."
#define ALLOC_ERR "Allocation failure: there was an Allocation Failure.\n"


/**
 * handling making first markov node in linked list
 * @param markov_chain a markov chain
 * @param data_ptr data for the new markov node
 * @return the new Node if succesfull, else - NULL
 */
Node *make_first_node (MarkovChain *markov_chain, void *data_ptr)
{
  MarkovNode *new_markov_node = calloc (1, sizeof (MarkovNode));
  if (!new_markov_node)
  {
    return NULL;
  }
  new_markov_node->frequencies_list = NULL;
  new_markov_node->lists_capacity = 0;
  new_markov_node->lists_len = 0;
  new_markov_node->frequencies_sum = 0;
  new_markov_node->data = markov_chain->copy_func (data_ptr);
  if (!new_markov_node->data)
  {
    free (new_markov_node);
    return NULL;
  }
  if (add (markov_chain->database, new_markov_node))
  {
    free (new_markov_node->data);
    free (new_markov_node);
    return NULL;
  }
  return markov_chain->database->first;
}
 //add a new node to database
Node *add_to_database (MarkovChain *markov_chain, void *data_ptr)
{
  if (!markov_chain || !data_ptr)
  {
    return NULL;
  }
  LinkedList *database = markov_chain->database;
  if (!database->first)
  {
    if (!make_first_node (markov_chain, data_ptr))
    {
      return NULL;
    }
    return markov_chain->database->first;
  }
  Node *node = database->first;
  while (node)
  {
    if (markov_chain->comp_func (data_ptr, node->data->data) == 0)
    {
      return node;
    }
    node = node->next;
  }
  MarkovNodeFrequency *new_markov_node_frequency_list = NULL;
  MarkovNode *new_markov_node =
      calloc (1, sizeof (*new_markov_node));
  if (!new_markov_node)
  {
    return NULL;
  }
  new_markov_node->frequencies_list = new_markov_node_frequency_list;
  new_markov_node->data = markov_chain->copy_func (data_ptr);
  if (!new_markov_node->data)
  {
    free (new_markov_node);
    return NULL;
  }
  new_markov_node->lists_len = 0;
  new_markov_node->lists_capacity = 0;
  new_markov_node->frequencies_sum = 0;
  if (add (markov_chain->database, new_markov_node))
  {
    free (new_markov_node->data);
    free (new_markov_node);
    return NULL;
  }
  return markov_chain->database->last;
}


//get a node from database
Node *get_node_from_database (MarkovChain *markov_chain, void *data_ptr)
{
  if (!markov_chain || !data_ptr)
  {
    return NULL;
  }
  Node *node = markov_chain->database->first;
  if (!node)
  {
    return NULL;
  }
  while (node)
  {
    if (markov_chain->comp_func (node->data->data, data_ptr) == 0)
    {
      return node;
    }
    node = node->next;
  }
  return NULL;
}

/**
 * create a new frequencies list
 * @param first_node the markov node to create freq-list for
 * @param second_node the first markov node in the new freq-list
 * @return true if success else NULL
 */
bool create_frequencies_list (MarkovNode *first_node, MarkovNode *second_node)
{
  first_node->frequencies_list = calloc (1, sizeof (MarkovNodeFrequency));
  if (!first_node->frequencies_list)
  {
    return false;
  }
  first_node->frequencies_list->markov_node = second_node;
  first_node->frequencies_list->frequency = 1;
  first_node->lists_len = 1;
  first_node->lists_capacity = 1;
  first_node->frequencies_sum = 1;
  return true;
}

//add a node to freq-list
bool add_node_to_frequencies_list (MarkovNode *first_node, MarkovNode
*second_node, MarkovChain *markov_chain)
{
  if (!first_node || !second_node || !markov_chain)
  {
    return NULL;
  }
  if (!first_node->frequencies_list)//if needed, create new frec-lst
  {
    return create_frequencies_list (first_node, second_node);
  }
  int i = 0;
  while (i < first_node->lists_len)
  {
    if (markov_chain->comp_func (((first_node->frequencies_list)
    [i].markov_node)->data, second_node->data) == 0)
    {
      first_node->frequencies_list[i].frequency += 1;
      first_node->frequencies_sum += 1;
      return true;
    }
    i++;
  }
  if (first_node->lists_len >= first_node->lists_capacity)
  {
    MarkovNodeFrequency *new_freq_node=realloc(first_node->frequencies_list
        , sizeof (MarkovNodeFrequency) *(first_node->lists_capacity + 1));
    if (!new_freq_node)
    {
      return false;
    }
    new_freq_node[(first_node->lists_len)].markov_node = second_node;
    new_freq_node[(first_node->lists_len)].frequency = 1;
    first_node->frequencies_list = new_freq_node;
    first_node->lists_capacity += 1;
    first_node->lists_len += 1;
    first_node->frequencies_sum += 1;
    return true;
  }
  first_node->frequencies_list[i + 1].markov_node = second_node;
  first_node->lists_len += 1;
  first_node->frequencies_sum += 1;
  return true;
}

//free the whole database
void free_database (MarkovChain **ptr_chain)
{
  if (!ptr_chain)
  {
    return;
  }
  if (*ptr_chain == NULL)
  {
    return;
  }
  Node *node = (*ptr_chain)->database->first;
  while (node)
  {
    (*ptr_chain)->free_data (node->data->data);//free data in markov node
    free (node->data->frequencies_list);//free freq-list in markov node
    free (node->data);//free markov node
    Node *prev = node;
    node = node->next;
    free (prev);//free node
  }
  free ((*ptr_chain)->database);
  free (*ptr_chain);
  *ptr_chain = NULL;
}


/**
 * get a random int between 1 to max_number
 * @param max_number max num to get
 * @return a random int
 */
int get_random_number (int max_number)
{
  return rand () % max_number;
}


/**
 * get the i'th markov node in the markov chain
 * @param markov_chain a markov chain
 * @param i an int
 * @return the i'th markov node if exists, otherwise - NULL.
 */
MarkovNode *get_ith_markov (MarkovChain *markov_chain, int i)
{
  Node *node = markov_chain->database->first;
  if (i == 0)
  {
    return node->data;
  }
  for (int j = 0; j < i; ++j)
  {
    node = node->next;
  }
  return node->data;
}


/**
 * get the first random node from the markov chain
 * @param markov_chain a markov chain
 * @return a random markov node
 */
MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int rand_n = get_random_number (markov_chain->database->size);
  MarkovNode *cur_markov = get_ith_markov (markov_chain, rand_n);
  while (markov_chain->is_last (cur_markov->data))//first word can't be last
  {
    rand_n = get_random_number (markov_chain->database->size);
    cur_markov = get_ith_markov (markov_chain, rand_n);
  }
  return cur_markov;
}


/**
 * given a markov node - returns a random markov node from the origins freq-lst
 * @param state_struct_ptr a given markov node.
 * @return a random markov node from the freq-list of the given markov node.
 */
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)//wasn't touched
{
  int n = get_random_number (state_struct_ptr->frequencies_sum);
  int counter = -1;
  int ind = 0;
  MarkovNodeFrequency *freq_node = state_struct_ptr->frequencies_list;
  do
  {
    counter += freq_node[ind].frequency;
    if (counter < n)
    {
      ind++;
    }
  }
  while (counter < n);
  return freq_node[ind].markov_node;
}


//generate new sequence from the markov chain, starting from first_node
//and not longer than max_length
void generate_tweet (MarkovChain *markov_chain,
                     MarkovNode *first_node, int max_length)
{
  if (!markov_chain || max_length < 2)
  {
    return;
  }
  if (!first_node)
  {
    first_node = get_first_random_node (markov_chain);
  }
  markov_chain->print_func (first_node->data);
  MarkovNode *cur_markov = first_node;
  int counter = 1;
  while ((!markov_chain->is_last (cur_markov->data)) && counter < max_length)
  {
    cur_markov = get_next_random_node (cur_markov);
    markov_chain->print_func(cur_markov->data);
    counter++;
  }
}
