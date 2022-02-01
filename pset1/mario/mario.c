#include <stdio.h>
#include <cs50.h>

int get_positive_int();

int main(void)
{
    //defining height and initial width
    int h = get_positive_int("Height: ");
    int w = 1;
    
    //looping through the pyramid "floors"
    for (int i = 0; i < h; i++)
    {
        //printing spaces to the left of the left side hashes
        for (int j = 0; j < h-w; j++)
        {
            printf(" ");
        }
        
        //printing left side hashes
        for (int j = 0; j < w; j++)
        {
            printf("#");
        }
        
        printf("  "); //printing space between the two sides of the pyramid
        
        //printing the hashes to the right
        for (int j = 0; j < w; j++)
        {
            printf("#");
        }

        printf("\n"); //breaking the line to get down to the floor below
        w++;
    }
}

//function on getting a positive only input from the user
int get_positive_int(string q)
{
    int positive_i;
    do
    {
        positive_i = get_int("%s", q);
    }while(positive_i < 1 || positive_i > 8);

    return positive_i;
}