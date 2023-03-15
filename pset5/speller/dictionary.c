// Implements a dictionary's functionality

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N * N];

// Initiallize word counter
unsigned int word_counter = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    unsigned int hash_number;
    // Get hash value of the word
    hash_number = hash(word);
    // Set up a trav pointer
    node *trav = table[hash_number];
    // Loop until trav points to NULL
    while (trav != NULL)
    {
        // Compare the word with trav->word in linked list
        if (strcasecmp(trav->word, word) == 0)
        {
            return true;
        }
        // Move trav to the next node
        trav = trav->next;
    }
    // If the function does not return in while loop, indicates no such word in dictionary
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // The hash table is designed to consider first 2 letters from word
    // Example: [0]~[25]:AA~AZ  [26]~[51]:BA~BZ
    // The first letter's index in alphet
    unsigned int first = toupper(word[0]) - 'A';
    // The second letter's index in alphet
    unsigned int second = toupper(word[1]) - 'A';

    if (strlen(word) == 1)
    {
        return 26 * first;
    }
    else
    {
        return 26 * first + second;
    }

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open dictionary file
    FILE *fp = fopen(dictionary, "r");
    // Check NULL
    if (fp == NULL)
    {
        return false;
    }

    // Create a buffer to store information
    char buffer[LENGTH + 1];

    // Read words from dictionary
    while (fscanf(fp, "%s", buffer) != EOF)
    {
        node *n = malloc(sizeof(node));
        // Check NULL
        if (n == NULL)
        {
            fclose(fp);
            return false;
        }
        // Copy words into nodes
        strcpy(n->word, buffer);

        // Load word into hash table
        // Get hash code
        unsigned int hash_number;
        hash_number = hash(n->word);

        // Insert a new node
        // If the word is the first word in the hash-table linked list
        if (table[hash_number] == NULL)
        {
            table[hash_number] = n;
            n->next = NULL;
            // Increase the counter in order to simplify the size function
            word_counter++;
        }
        else
        {
            n->next = table[hash_number];
            table[hash_number] = n;
            // Increase the counter in order to simplify the size function
            word_counter++;
        }
    }
    // Close file
    fclose(fp);
    // Successfully loaded
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    // Simply return word_counter
    return word_counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *trav = NULL;
    node *temp = NULL;
    for (int i = 0; i < N * N; i++)
    {
        // If hash table index i has values in it
        if (table[i] != NULL)
        {
            trav = table[i];
            do
            {
                // Let temp = trav in order to memorize the location of current node
                temp = trav;
                // Move trav forward to point to the next node
                trav = trav->next;
                // Free temp
                free(temp);
                // Notice: Now trav is pointing to one location forward, free(temp) will not affect rest of the list
            } while (trav != NULL);
        }
    }
    // Successfully unloaded
    return true;
}
