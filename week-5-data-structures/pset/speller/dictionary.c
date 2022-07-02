// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

bool insert_word(char *word);
bool insert_next(node *prev, char *word, int index);
void free_node(node *n);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

int dict_word_count = 0;

// Hash table
node *table;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    table = calloc(N, sizeof(node));
    char c;
    FILE *dict = fopen(dictionary, "r");

    bool loaded = false;
    int index = 0;
    char word[LENGTH + 1];

    while (fread(&c, sizeof(char), 1, dict))
    {
        if (c != '\n')
        {
            word[index] = c;
            index++;
        }
        else
        {
            word[index] = '\0';
            loaded = insert_word(word);

            // if a word could not be loaded, break
            if (!loaded)
            {
                break;
            }

            index = 0;
            dict_word_count++;
        }
    }

    fclose(dict);
    free(table);
    return loaded;
}

bool insert_word(char *word)
{
    int hash_index = hash(word);
    bool inserted = false;

    if (table[hash_index].word[0] == '\000')
    {
        strcpy(table[hash_index].word, word);
        return true;
    }
    else
    {
        inserted = insert_next(&table[hash_index], word, hash_index);
        return inserted;
    }

    return inserted;
}

bool insert_next(node *prev, char *word, int index)
{
    node *next = prev->next;
    if (next == NULL)
    {
        node *new_node = calloc(1, sizeof(node));
        if (!new_node)
        {
            return false;
        }

        strcpy(new_node->word, word);
        prev->next = new_node;
        return true;
    }
    else
    {
        return insert_next(next, word, index);
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dict_word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        free_node(&table[i]);
    }
    return false;
}

void free_node(node *n)
{
    if (n->next == NULL)
    {
        free(n);
        return;
    }

    free_node(n->next);
}