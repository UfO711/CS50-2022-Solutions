#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// A char variable can be stored into an int array, i.e. the corresponding ASCII number
// (char)int will print the corresponding character refers to int in ASCII table

int main(int argc, string argv[])
{
    // Check whether the number of argument is 2
    if (argc == 2)
    {
        // Check the length of the second argument
        if (strlen(argv[1]) == 26)
        {
            // Check wheather they are all alphabetic characters
            for (int i = 0; i < 26; i++)
            {
                // If a non-alphabetic character detected, exit
                if (!isalpha(argv[1][i]))
                {
                    printf("Key must only contain alphabetic characters.\n");
                    return 1;
                }
            }
            // Check wheather it contains repeated characters
            for (int i = 0; i < 25; i++)
            {
                for (int j = i + 1; j < 26; j++)
                {
                    // Change the character to lower case to compare,
                    if (tolower(argv[1][i]) == tolower(argv[1][j]))
                    {
                        printf("Key must not contain repeated characters.\n");
                        return 1;
                    }
                }
            }
            // The input argument is valid
            string plaintext = get_string("plaintext: ");
            // Create an array to store the ASCII number of each character in plaintext
            int arr[strlen(plaintext)];
            // The alpha_index th letter in alphabet
            int alpha_index;
            for (int i = 0; i < strlen(plaintext); i++)
            {
                // Check wheather the character is an alphabetic character or not
                if (isalpha(plaintext[i]))
                {
                    // If the i th character in plaintext is an upper case letter
                    if (isupper(plaintext[i]))
                    {
                        // Calculate the index of this letter in alphabet
                        alpha_index = plaintext[i] - 65;
                        // Store the ASCII number of the uppercase of the letter in alpha_index th place in argv[]
                        arr[i] = toupper(argv[1][alpha_index]);
                    }
                    else
                    {
                        // Symmetrically for the lowercase letter in plaintext
                        alpha_index = plaintext[i] - 97;
                        arr[i] = tolower(argv[1][alpha_index]);
                    }
                }
                // For non-alphabetic character, store the ASCII number directly into arr[]
                else
                {
                    arr[i] = plaintext[i];
                }
            }
            // Print the arr[], after casting it to char type
            printf("ciphertext: ");
            for (int i = 0; i < strlen(plaintext); i++)
            {
                printf("%c", (char)arr[i]);
            }
            printf("\n");
            return 0;
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }
}