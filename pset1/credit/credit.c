#include <cs50.h>
#include <stdio.h>
#include <math.h>

int luhn_algo(long int n, int num_digit)
{
    int sum;
    for (int i = 1; i <= num_digit; i++)
    {
        // Calculate the digit, start from the tail
        int digit = (n % (long int)pow(10, i)) / pow(10, (i - 1));
        // Digits need to be multiplied by 2
        if (i % 2 == 0)
        {
            // Digits that smaller than 4 can apply addition directly
            if (digit <= 4)
            {
                sum = sum + digit * 2;
            }
            // Digits that greater than 4 become 2-digit number after multiplied by 2
            // Calculate the sum of all digits after multiplied by 2
            else
            {
                sum = sum + 1 + (digit - 5) * 2;
            }
        }
        // Digits do not operate multiplication
        else
        {
            sum += digit;
        }
    }
    // Return the reminder of sum divided by 10
    return sum % 10;
}

int main(void)
{
    long int number;
    int num_digit = 0;
    int first_digit;
    int first_two_digit;

    // Prompt for the card number
    do
    {
        number = get_long("Number: ");
    }
    while (number < 0);

    // Count digits
    for (long int i = number; i > 0; i /= 10)
    {
        num_digit++;
    }

    // Calculate the first digit and the first two digits
    first_digit = number / pow(10, (num_digit - 1));
    first_two_digit = number / pow(10, (num_digit - 2));

    // If the number of digit is valid
    if (num_digit == 13 || num_digit == 15 || num_digit == 16)
    {
        // If the number pass the luhn's algorithm test
        if (luhn_algo(number, num_digit) == 0)
        {
            if (num_digit == 13 && first_digit == 4)
            {
                printf("VISA\n");
            }
            else if (num_digit == 15 && (first_two_digit == 34 || first_two_digit == 37))
            {
                printf("AMEX\n");
            }
            else if (first_digit == 4)
            {
                printf("VISA\n");
            }
            else if (first_two_digit == 51 || first_two_digit == 52 || first_two_digit == 53 || first_two_digit == 54 || first_two_digit == 55)
            {
                printf("MASTERCARD\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    
}
