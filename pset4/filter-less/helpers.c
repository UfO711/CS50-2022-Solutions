#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg_rgb;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average value
            avg_rgb = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg_rgb;
            image[i][j].rgbtGreen = avg_rgb;
            image[i][j].rgbtRed = avg_rgb;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sepiaRed;
    int sepiaGreen;
    int sepiaBlue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the sepiaed RGB
            sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);

            // Make sure the result is no higher than 255
            if (sepiaRed > 255)
                image[i][j].rgbtRed = 255;
            else
                image[i][j].rgbtRed = sepiaRed;
            if (sepiaGreen > 255)
                image[i][j].rgbtGreen = 255;
            else
                image[i][j].rgbtGreen = sepiaGreen;
            if (sepiaBlue > 255)
                image[i][j].rgbtBlue = 255;
            else
                image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < (width / 2); j++)
        {
            // 0 1 2 3 4 5
            // Exchange pixel0 with pixel5
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    // Create a copy of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    
    // Calculate image[0][0], divided by 3.0
    image[0][0].rgbtRed = round((copy[0][0].rgbtRed + copy[0][1].rgbtRed + copy[1][0].rgbtRed + copy[1][1].rgbtRed) / 4.0);
    image[0][0].rgbtGreen = round((copy[0][0].rgbtGreen + copy[0][1].rgbtGreen + copy[1][0].rgbtGreen + copy[1][1].rgbtGreen) / 4.0);
    image[0][0].rgbtBlue = round((copy[0][0].rgbtBlue + copy[0][1].rgbtBlue + copy[1][0].rgbtBlue + copy[1][1].rgbtBlue) / 4.0);

    // Calculate the pixel in the upper-right corner, divided by 3.0
    image[0][width-1].rgbtRed = round((copy[0][width-2].rgbtRed + copy[0][width-1].rgbtRed + copy[1][width-2].rgbtRed + copy[1][width-1].rgbtRed) / 4.0);
    image[0][width-1].rgbtGreen = round((copy[0][width-2].rgbtGreen + copy[0][width-1].rgbtGreen + copy[1][width-2].rgbtGreen + copy[1][width-1].rgbtGreen) / 4.0);
    image[0][width-1].rgbtBlue = round((copy[0][width-2].rgbtBlue + copy[0][width-1].rgbtBlue + copy[1][width-2].rgbtBlue + copy[1][width-1].rgbtBlue) / 4.0);

    // Calculate the pixel in the lower-left corner, divided by 3.0
    image[height-1][0].rgbtRed = round((copy[height-2][0].rgbtRed + copy[height-2][1].rgbtRed + copy[height-1][0].rgbtRed + copy[height-1][1].rgbtRed) / 4.0);
    image[height-1][0].rgbtGreen = round((copy[height-2][0].rgbtGreen + copy[height-2][1].rgbtGreen + copy[height-1][0].rgbtGreen + copy[height-1][1].rgbtGreen) / 4.0);
    image[height-1][0].rgbtBlue = round((copy[height-2][0].rgbtBlue + copy[height-2][1].rgbtBlue + copy[height-1][0].rgbtBlue + copy[height-1][1].rgbtBlue) / 4.0);

    // Calculate the pixel in the lower-right corner, divided by 3.0
    image[height-1][width-1].rgbtRed = round((copy[height-2][width-2].rgbtRed + copy[height-2][width-1].rgbtRed + copy[height-1][width-2].rgbtRed + copy[height-1][width-1].rgbtRed) / 4.0);
    image[height-1][width-1].rgbtGreen = round((copy[height-2][width-2].rgbtGreen + copy[height-2][width-1].rgbtGreen + copy[height-1][width-2].rgbtGreen + copy[height-1][width-1].rgbtGreen) / 4.0);
    image[height-1][width-1].rgbtBlue = round((copy[height-2][width-2].rgbtBlue + copy[height-2][width-1].rgbtBlue + copy[height-1][width-2].rgbtBlue + copy[height-1][width-1].rgbtBlue) / 4.0);

    // Calculate the pixels on the upper edge, divided by 6.0
    for (int j = 1; j < width - 1; j++)
    {
        image[0][j].rgbtRed = round((copy[0][j-1].rgbtRed + copy[0][j].rgbtRed + copy[0][j+1].rgbtRed + copy[1][j-1].rgbtRed + copy[1][j].rgbtRed + copy[1][j+1].rgbtRed) / 6.0);
        image[0][j].rgbtGreen = round((copy[0][j-1].rgbtGreen + copy[0][j].rgbtGreen + copy[0][j+1].rgbtGreen + copy[1][j-1].rgbtGreen + copy[1][j].rgbtGreen + copy[1][j+1].rgbtGreen) / 6.0);
        image[0][j].rgbtBlue = round((copy[0][j-1].rgbtBlue + copy[0][j].rgbtBlue + copy[0][j+1].rgbtBlue + copy[1][j-1].rgbtBlue + copy[1][j].rgbtBlue + copy[1][j+1].rgbtBlue) / 6.0);
    }
    // Calculate the pixels on the left edge, divided by 6.0
    for (int i = 1; i < height - 1; i++)
    {
        image[i][0].rgbtRed = round((copy[i-1][0].rgbtRed + copy[i-1][1].rgbtRed + copy[i][0].rgbtRed + copy[i][1].rgbtRed + copy[i+1][0].rgbtRed + copy[i+1][1].rgbtRed) / 6.0);
        image[i][0].rgbtGreen = round((copy[i-1][0].rgbtGreen + copy[i-1][1].rgbtGreen + copy[i][0].rgbtGreen + copy[i][1].rgbtGreen + copy[i+1][0].rgbtGreen + copy[i+1][1].rgbtGreen) / 6.0);
        image[i][0].rgbtBlue = round((copy[i-1][0].rgbtBlue + copy[i-1][1].rgbtBlue + copy[i][0].rgbtBlue + copy[i][1].rgbtBlue + copy[i+1][0].rgbtBlue + copy[i+1][1].rgbtBlue) / 6.0);
    }

    // Calculate the pixels on the bottom edge, divided by 6.0
    for (int j = 1; j < width - 1; j++)
    {
        image[height-1][j].rgbtRed = round((copy[height-2][j-1].rgbtRed + copy[height-2][j].rgbtRed + copy[height-2][j+1].rgbtRed + copy[height-1][j-1].rgbtRed + copy[height-1][j].rgbtRed + copy[height-1][j+1].rgbtRed) / 6.0);
        image[height-1][j].rgbtGreen = round((copy[height-2][j-1].rgbtGreen + copy[height-2][j].rgbtGreen + copy[height-2][j+1].rgbtGreen + copy[height-1][j-1].rgbtGreen + copy[height-1][j].rgbtGreen + copy[height-1][j+1].rgbtGreen) / 6.0);
        image[height-1][j].rgbtBlue = round((copy[height-2][j-1].rgbtBlue + copy[height-2][j].rgbtBlue + copy[height-2][j+1].rgbtBlue + copy[height-1][j-1].rgbtBlue + copy[height-1][j].rgbtBlue + copy[height-1][j+1].rgbtBlue) / 6.0);
    }

    // Calculate the pixels on the right edge, divided by 6.0
    for (int i = 1; i < height - 1; i++)
    {
        image[i][width-1].rgbtRed = round((copy[i-1][width-2].rgbtRed + copy[i-1][width-1].rgbtRed + copy[i][width-2].rgbtRed + copy[i][width-1].rgbtRed + copy[i+1][width-2].rgbtRed + copy[i+1][width-1].rgbtRed) / 6.0);
        image[i][width-1].rgbtGreen = round((copy[i-1][width-2].rgbtGreen + copy[i-1][width-1].rgbtGreen + copy[i][width-2].rgbtGreen + copy[i][width-1].rgbtGreen + copy[i+1][width-2].rgbtGreen + copy[i+1][width-1].rgbtGreen) / 6.0);
        image[i][width-1].rgbtBlue = round((copy[i-1][width-2].rgbtBlue + copy[i-1][width-1].rgbtBlue + copy[i][width-2].rgbtBlue + copy[i][width-1].rgbtBlue + copy[i+1][width-2].rgbtBlue + copy[i+1][width-1].rgbtBlue) / 6.0);
    }

    // Calculate the midle pixels, divided by 9.0
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            image[i][j].rgbtRed = round((copy[i-1][j-1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed + copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j-1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed) / 9.0);
            image[i][j].rgbtGreen = round((copy[i-1][j-1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j-1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen) / 9.0);
            image[i][j].rgbtBlue = round((copy[i-1][j-1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j-1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue) / 9.0);
        }
    }
    return;
}
