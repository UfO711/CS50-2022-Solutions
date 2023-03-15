#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    // if (isalpha(argv[1][0]))
    // {
    //     printf("pass\n");
    //     return 0;
    // }
    // else
    // {
    //     return 1;
    // }

    // for (int i = 0; i < 25; i++)
    // {
    //     for (int j = 1; j < 26; j++)
    //     {
    //         if (argv[1][i] == argv[1][j])
    //         {
    //             printf("Key must not contain repeated characters.\n");
    //             return 1;
    //         }
    //     }
    // }
    //         // The input argument is valid

    // printf("pass\n");

    string plaintext = get_string("plaintext: ");
    int arr[strlen(plaintext)];
    int alpha_index;
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                alpha_index = plaintext[i] - 65;
                arr[i] = toupper(argv[1][alpha_index]);
            }
            else
            {
                alpha_index = plaintext[i] - 97;
                arr[i] = tolower(argv[1][alpha_index]);
            }
        }
        else
        {
            arr[i] = plaintext[i];
        }
    }
    for (int i = 0; i < strlen(plaintext); i++)
    {
        printf("%c", (char)arr[i]);
    }
    printf("\n");
    // printf("%d\n", arr[0]);
}