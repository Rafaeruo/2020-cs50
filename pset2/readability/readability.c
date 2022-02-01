#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

float calculate_index(string text);

int main(void)
{
    string text = get_string("Text: ");

    //defines index by rounding the fucntion's output
    int g_index = (int) round(calculate_index(text));

    //checks index and prints accordingly
    if (g_index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (g_index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", g_index);
    }

}

float calculate_index(string text)
{
    int characters = 0;
    int words = 1; //not 0 cause the program will count every space as a new word, but there are no spaces before the first word.
    int sentences = 0;
    char current_character;

    //checks for the number of letters, words and sentences, respectively
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        current_character = text[i];

        if ((current_character >= 'a' && current_character <= 'z') || (current_character >= 'A' && current_character <='Z'))//each char is represetning their number in the ascii table, threfore being able to take part in comparisons
        {
            characters++;
        }
        else if (current_character == 32)//32 is equal to the sapce key in the ascii table (might as well have put " " instead, same result)
        {
            words++;
        }
        else if ((current_character == '.' && text[i-1] != '.') || (current_character == '!' && text[i-1] != '!') || (current_character == '?' && text[i-1] != '?'))
        {
            sentences++;
        }

    }

    //printf("words:%i\n", words);
    //printf("letters:%i\n", characters);
    //printf("sentences:%i\n", sentences);


    //calculates average of letters and sentences in 100 words
    float x = 100.0 / (float) words;
    //printf("x: %f\n", x);
    float avg_c = characters * x;
    float avg_s = sentences * x;

    //printf("average letters:%f\n", avg_c);
    //printf("average sentences:%f\n", avg_s);

    return 0.0588 * avg_c - 0.296 * avg_s - 15.8;



}