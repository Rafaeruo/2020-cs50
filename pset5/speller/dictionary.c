// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"


//My prototypes:
int findBucket(const char *word);
void printTable();

//My Globals
int tableSize = 0;


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1300;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int bucket = hash(word);//looks for the index to look for the word

    node *current_node = table[bucket];//where the address for the next node will be getting stored through the iterations

    while(current_node != NULL)//while the address for the next node isn't null, meaning it doesnt exist
    {
        if (strcasecmp(current_node->word, word) == 0)//if the word in the current node is equal to the word we're looking for, return true
        {
            return true;
        }
        current_node = current_node->next; //else, look in the next node
    }

    return false;//after looking throigh the entire hash table and not finding the word, return false
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int len = strlen(word);
    char *flatword = malloc(sizeof(char)*(len+1));

    strcpy(flatword, word);
    int bucket = 0;
    int charcode = 0;

    for (int i = 0; i < len; i++)
    {
        if (isalpha(flatword[0]))
        {
            flatword[i] = toupper(flatword[i]);
            charcode = flatword[i]-64;//a=1, b=2, z=27
            bucket += charcode;
        }
    }

    free(flatword);
    if (bucket > N-1)//if the hash index ends up larger than the maximum index for the table, do the following
    {
        return bucket % N;
    }
    return bucket;

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *f = fopen(dictionary, "r");
    if (f == NULL)
    {
        return false;
    }



    node *current_node = malloc(sizeof(node));//where the node stars read will be store through the iterations
    current_node->next = NULL;


    while (fscanf(f, "%s", current_node->word) != EOF)//keeps setting words to the *current nodee
    {

        int bucket = hash(current_node->word);//finds a bucket in the hash table according to the hash function

        node *next_node = table[bucket];//finds the first node in the bucket

        if (next_node == NULL)//if there is no node in the bucket, a node with the word in the current_node should be created
        {
            table[bucket] = malloc(sizeof(node));
            table[bucket]->next = NULL;
            strcpy(table[bucket]->word, current_node->word);
        }else//if there already is a node in the bucket, a new node should be added to the start of the hash table
        {
            current_node->next = table[bucket];
            table[bucket] = current_node;
            current_node = malloc(sizeof(node));
        }

        tableSize++;//keeping track of the amount of words in the dictionary
    }


    //printTable();


    free(current_node);//freeing the adress where the the node with the words of the dictionary were first being stored through the iterations
    fclose(f);//closing the dictionary file
    return true;

    return false;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return tableSize;//Simply returns the tableSize global variable that was calculated during the loading function execution
    return 0;ye
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *current_node = NULL;//where the address to the current node will be stored and changed through the iterations
    node *tmp = NULL;//where the addresses to the next nodes will be stored temporarily while freeing the current node
    for (int i = 0; i < N; i++)//iterate through the entire hash table
    {
        current_node = table[i];//get the address of the current node into current_node

        while (current_node != NULL)//while the linked list isnt over, continue freeing
        {
            tmp = current_node->next;//saving the address to the next node temporarily
            free(current_node);//freeing the current node
            current_node = tmp;//setting the address of the next node to the current_node
        }
    }

    return true;
}



void printTable()
{
    //printing the dictionary so I can look for errors in the process
    node *nprint = NULL;

    for (int i = 0; i < N; i++)
    {
        nprint = table[i];
        if (table[i] != NULL)
        {
            while(nprint!=NULL)
            {
                printf("%s\n", nprint->word);
                nprint = nprint->next;
            }
        }
    }
}