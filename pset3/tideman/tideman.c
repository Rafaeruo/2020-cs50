#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool has_cycle(int winner, int loser);

int main(int argc, string argv[])//the main function was pre-made
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        
        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //loops through the candidates list
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)//checks if the name is in this position in the list of candidates
        {
            ranks[rank] = i; //if it is, put the number corresponding to the name of the candidate (i) in the ranks list in the ith (rank) position
            return true;
        }
    }
    return false;//the name is incorrect
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    //loops through the ranks list and adds the preferences over to the preferences list
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i+1; j < candidate_count; j++)//i + 1 because the ones that come after i are the ones that lose to i in terms of preference
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int count = 0;//only serves the purpose of setting the value of the pair_count variable later on
    //goes through the preferences array without repeating pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i+1; j < candidate_count; j++)// j = i+1 to not repeat pairs (i.e. AB and BA)
        {
            pair p;
            if (preferences[i][j] > preferences[j][i])//check reverse to see which has more preferences over the other
            {
                p.winner = i;
                p.loser = j;
                pairs[count] = p;
                count ++;
            }
            else if (preferences[i][j] < preferences[j][i])//same as before but with winner and loser values reversed
            {
                p.winner = j;
                p.loser = i;
                pairs[count] = p;
                count ++;
            }
            
        }
    }
    pair_count = count;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp;
    int count;
    //basically a bubble sort through the pairs list
    for (int j = 0; j < pair_count; j++)
    {
        count = 0;
        for (int i = 0; i < pair_count; i++)
        {   
            //, checking their strenghts in the preferences array
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i+1].winner][pairs[i+1].loser])
            {
                temp = pairs[i];
                pairs[i] = pairs[i+1];
                pairs[i+1] = temp;
                count++;
            }
            if (count == 0)
            {
                return;
            }
        }
    }
    
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int winner;
    int loser;
    //recursive way of locking pairs
    for (int i = 0; i < pair_count; i++)
    {
        winner = pairs[i].winner;
        loser = pairs[i].loser;
        
        if (!has_cycle(winner, loser))//checks if each pair will create a cycle or not. if not, lock the pair.
        {
            locked[winner][loser] = true;
        }
    }
    return;
}

bool has_cycle(int winner, int loser)//used in the lock_pairs function, it checks for "cycle creating pairs"
{
    int current = 0;
    //check which candidates the loser wins over. if it is the winner, then created cycle. if not, it calls itself with a new "loser" (recursively)
    while (current < candidate_count)
    {
        if (locked[loser][current] == true)
        {
            if (current == winner)
            {
                return true;
            }
            else
            {
                if (has_cycle(winner, current))
                {
                    return true;
                }
            }
        }
        current++;
    }
    
    return false;
    
}

// Print the winner of the election
void print_winner(void)
{
    //basically goes through the locked array and checks for candidates that dont lose to anyone, hence the variable count
    int count;
    for (int i = 0; i < candidate_count; i++)
    {
        count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                count++;
                break;
            }
        }
        if (count == 0)
        {
            printf("%s\n", candidates[i]);
            return; 
        }
    }
    return;
}

