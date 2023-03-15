#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    // printf("%d letters\n", count_letters(text));
    // printf("%d words\n", count_words(text));
    // printf("%d sentences\n", count_sentences(text));

    int num_letters = count_letters(text);
    int num_words = count_words(text);
    int num_sentences = count_sentences(text);

    float L = ((float)num_letters / (float)num_words) * 100;
    float S = ((float)num_sentences / (float)num_words) * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;
    int rounded_index = round(index);

    if (rounded_index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (rounded_index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", rounded_index);
    }


}

int count_letters(string text)
{
    int num_letter = 0;
    // Iterate through every character of the text
    for (int i = 0; i < strlen(text); i++)
    {
        // If it is an alphabetical character, increments num_letter
        if (isalpha(text[i]))
        {
            num_letter++;
        }
    }
    return num_letter;
}

int count_words(string text)
{
    int num_white_space = 0;
    // Iterate through every character of the text
    for (int i = 0; i < strlen(text); i++)
    {
        // If it is a white space, increments num_white_space
        if (isspace(text[i]))
        {
            num_white_space++;
        }
    }
    // We have one more word than the number of white space
    return num_white_space + 1;
}

int count_sentences(string text)
{
    int num_sentence = 0;
    // Iterate through every character of the text
    for (int i = 0; i < strlen(text); i++)
    {
        // If the ASCII code of the character matches (. ! ?) and the character is not a whitesapce
        if (text[i] != ' ' && (text[i] == 46 || text[i] == 32 || text[i] == 63))
        {
            num_sentence++;
        }
    }
    return num_sentence;
}