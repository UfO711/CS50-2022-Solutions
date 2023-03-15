#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // Check if the height is within the range
    do
    {
        height = get_int("Height: ");
    }
    while (height > 8 || height < 1);

    // For row
    for (int i = 0; i < height; i++)
    {
        // For column to print the space
        for (int j = height - i - 1; j > 0; j--)
        {
            printf(" ");
        }

        // For column to print the bricks
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }

        // To print the double space in between
        printf("  ");

        // For column to print the other side of bricks
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}