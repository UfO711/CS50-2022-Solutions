#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool has_cycle(int s, int d);

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

int main(int argc, string argv[])
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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // **********SOLVED QUESTION: what about those cells in the matix with the value 0? what if the voter all give the same ranks, it will leads to most cells in preferences blank or untouched*************
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Pass the cell when where, for instance, 0th candidate is over 0th candidate
            if (preferences[i][j] != preferences[j][i])
            {
                pair temp;
                if (preferences[i][j] > preferences[j][i])
                {
                    temp.winner = i;
                    temp.loser = j;
                }
                else
                {
                    temp.winner = j;
                    temp.loser = i;
                }
                // SOLVED QUESTION: WITHOUT INITIALIZATION HOW CAN WE LOCATE PAIR_COUNT
                pairs[pair_count] = temp;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
// In this function, I assume that the winner who has the most people prefered will rank no 1 in the list. Although it accidentally passes the check50, more steps are needed to calculate the margin, and rank them based on the margin
void sort_pairs(void)
{
    pair temp_pairs;
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = i + 1; j < pair_count; j++)
        {
            // Selection sort
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                temp_pairs = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp_pairs;
            }
        }
    }
    return;
}

// Check wheater the newly added edge creates a cycle
// Any backtraced step == destination of the loser -> a cycle
bool has_cycle(int s, int d)
{
    // s means source, d means destination
    while (s != -1 && s != d)
    {
        bool found = false;
        for (int i = 0; i < candidate_count; i++)
        {
            // Loop over all candidates, if there is an ith candidate points to the source (winner) of newly added edge, then
            // Render found to true, this will take effect when we break the for loop and encounters the if statement. A true value will not accepted by the if condition, which is reasonable because we have to continue the backtrace operation, otherwise the while loop will break
            // And set the new source to ith candidates, in this step, we actually find a path from i to s. Then we set i as the new source to forward the backtrace
            // NOTICE: our goal is to perform the backtrace anyway until we find a source which is equal to the d (the parameter) or lose the connection (cannot find a ith candidate point to s)
            if (locked[i][s])
            {
                found = true;
                s = i;
            }
        }
        // If there is no candidate point to the source (winner), set s = -1 which will stop the loop (notice the while statement: s != -1)
        // "no candidate point to" means from the source, there is no destination to traceback to, therefore, the pramater: s -> d will not create a cycle
        if (!found)
        {
            s = -1;
        }
    }
    // The while loop may satisfy two condition to break
    // One is when s == d, which means we finally traceback to the ith candidate that is the same as the pramater d which indicates there would have been a cycle appear if we have added s -> d (the parameter). That is because by performing the above code, we have found a path from the ith candidate to the source (the prameter). And this path compined with path s -> d (the prameters, also remember in this case d = ith candidate) will create a cycle
    if (s == d)
    {
        return true;
    }
    // The other is when s == -1, which means found == false, means the path searching is failed. So, the newly added path s -> d will not create a cycle, then return false.
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!has_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // NOTICE the using a bool found_source: found_source
    for (int i = 0; i < pair_count; i++)
    {
        bool found_source = true;
        for (int j = 0; j < pair_count; j++)
        {
            // Visualize the locked matrix first and consider that once there is column has all fase value, which means on edge points to it, the column index is indeed a source
            if (locked[j][i] == true)
            {
                found_source = false;
                break;
            }
        }
        // If the last if-staement is not satisfied, the found_source will be true, then just print the ith candidate
        if (found_source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}