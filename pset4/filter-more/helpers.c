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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a new image with 1 pixel solid black border around the edge
    int new_height = height + 2;
    int new_width = width + 2;
    RGBTRIPLE copy_with_border[new_height][new_width];
    BYTE black = 0x00;

    // Initialize the new image
    for (int i = 0; i < new_height; i++)
    {
        for (int j = 0; j < new_width; j++)
        {
            copy_with_border[i][j].rgbtRed = black;
            copy_with_border[i][j].rgbtGreen = black;
            copy_with_border[i][j].rgbtBlue = black;
        }
    }

    // Replace the middle of the (RGBTRIPLE)copy_with_border with pixels in (RGBTRIPLE)image
    for (int i = 1; i < new_height - 1; i++)
    {
        for (int j = 1; j < new_width - 1; j++)
        {
            copy_with_border[i][j] = image[i-1][j-1];
        }
    }

    int red_gx;
    int red_gy;
    int green_gx;
    int green_gy;
    int blue_gx;
    int blue_gy;

    for (int i = 1; i < new_height - 1; i++)
    {
        for (int j = 1; j < new_width - 1; j++)
        {
            // Applying Sobel operator
            red_gx = copy_with_border[i-1][j-1].rgbtRed * -1 + copy_with_border[i-1][j+1].rgbtRed - 2 * copy_with_border[i][j-1].rgbtRed + 2 * copy_with_border[i][j+1].rgbtRed - copy_with_border[i+1][j-1].rgbtRed + copy_with_border[i+1][j+1].rgbtRed;
            red_gy = copy_with_border[i-1][j-1].rgbtRed * -1 - 2 * copy_with_border[i-1][j].rgbtRed - copy_with_border[i-1][j+1].rgbtRed + copy_with_border[i+1][j-1].rgbtRed + 2 * copy_with_border[i+1][j].rgbtRed + copy_with_border[i+1][j+1].rgbtRed;
            int new_red = round(pow((red_gx * red_gx + red_gy * red_gy), .5));

            // Make sure the updated pixel value in no higher than 255
            if (new_red > 255)
                image[i-1][j-1].rgbtRed = 255;
            else
                image[i-1][j-1].rgbtRed = new_red;

            // Applying Sobel operator
            green_gx = copy_with_border[i-1][j-1].rgbtGreen * -1 + copy_with_border[i-1][j+1].rgbtGreen - 2 * copy_with_border[i][j-1].rgbtGreen + 2 * copy_with_border[i][j+1].rgbtGreen - copy_with_border[i+1][j-1].rgbtGreen + copy_with_border[i+1][j+1].rgbtGreen;
            green_gy = copy_with_border[i-1][j-1].rgbtGreen * -1 - 2 * copy_with_border[i-1][j].rgbtGreen - copy_with_border[i-1][j+1].rgbtGreen + copy_with_border[i+1][j-1].rgbtGreen + 2 * copy_with_border[i+1][j].rgbtGreen + copy_with_border[i+1][j+1].rgbtGreen;
            int new_green = round(pow((green_gx * green_gx + green_gy * green_gy), .5));

            // Make sure the updated pixel value in no higher than 255
            if (new_green > 255)
                image[i-1][j-1].rgbtGreen = 255;
            else
                image[i-1][j-1].rgbtGreen = new_green;

            // Applying Sobel operator
            blue_gx = copy_with_border[i-1][j-1].rgbtBlue * -1 + copy_with_border[i-1][j+1].rgbtBlue - 2 * copy_with_border[i][j-1].rgbtBlue + 2 * copy_with_border[i][j+1].rgbtBlue - copy_with_border[i+1][j-1].rgbtBlue + copy_with_border[i+1][j+1].rgbtBlue;
            blue_gy = copy_with_border[i-1][j-1].rgbtBlue * -1 - 2 * copy_with_border[i-1][j].rgbtBlue - copy_with_border[i-1][j+1].rgbtBlue + copy_with_border[i+1][j-1].rgbtBlue + 2 * copy_with_border[i+1][j].rgbtBlue + copy_with_border[i+1][j+1].rgbtBlue;
            int new_blue = round(pow((blue_gx * blue_gx + blue_gy * blue_gy), .5));

            // Make sure the updated pixel value in no higher than 255
            if (new_blue > 255)
                image[i-1][j-1].rgbtBlue = 255;
            else
                image[i-1][j-1].rgbtBlue = new_blue;
        }
    }
    return;
}
