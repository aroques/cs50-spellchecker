/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <cs50.h>

#include "dictionary.h"

// one extra for apostrophe
#define SIZE 27

// Number of words loaded into dictionary
unsigned int numLoaded = 0;

// Global Varibles
int tableSize;
node* hashtable[SIZE];

/**
 *  Hash Function.
 */
int hash(char* key)
{
    int hash = toupper(key[0]) - 'A';
    // Check for apostrophe
    if (hash == 31)
    {
        hash = 26;
    }
    return hash % SIZE;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // Create variable to hold const char* word copy
    int n = strlen(word);
    char wordCpy[n + 1];
    
    // Copy const char* and make it lowercase
    for (int i = 0; i < n; i++)
    {
        wordCpy[i] = word[i];
        wordCpy[i] = tolower(wordCpy[i]);
    }
    // Add null zero
    wordCpy[n] = '\0';
    
    // Get hashkey
    int hashkey = hash(wordCpy);
    
    // Check to see if word is in dictionary
    node* cursor = hashtable[hashkey];
    
    while (cursor != NULL)
    {
        if (strcmp(wordCpy, cursor->word) == 0)
        {
            return true;
        }
        
        cursor = cursor->next;
    }
    
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Open dictionary
    FILE* inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        printf("Could not open dictionary.\n");
        return false;
    }
    
    // Create a new node to act as buffer for current word
    node* wordBuffer = malloc(sizeof(node));
    
    // Read a line from dictionary into new node
    while(true)
    {
        if (fscanf(inptr, "%s", wordBuffer->word) == 1)
        {
            // Create a new node
            node* newNode = malloc(sizeof(node));
            newNode->next = NULL;
            
            // Copy word to new node
            strcpy(newNode->word, wordBuffer->word);
            
            // Get hash key
            int hashKey = hash(wordBuffer->word);
            
            // Add word to hash table
            newNode->next = hashtable[hashKey];
            hashtable[hashKey] = newNode;
            
            // Increment number of words loaded
            numLoaded++;
        }
        else
        {
            break;
        }
    }
    
    free(wordBuffer);
    fclose(inptr);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return numLoaded;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < SIZE; i++)
    {
        node* cursor = hashtable[i];
        while(cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    
    return true;
}
