#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

BYTE clamp_color(int target);
RGBTRIPLE get_box_average(int height, int width, int center_h, int center_w, RGBTRIPLE image[height][width],
                          RGBTRIPLE original_image[height][width]);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int sum = image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed;
            int average = round(sum / 3.0);
            image[h][w].rgbtBlue = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            BYTE originalRed = image[h][w].rgbtRed;
            BYTE originalGreen = image[h][w].rgbtGreen;
            BYTE originalBlue = image[h][w].rgbtBlue;

            image[h][w].rgbtRed = clamp_color(round(0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue));
            image[h][w].rgbtGreen = clamp_color(round(0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue));
            image[h][w].rgbtBlue = clamp_color(round(0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue));
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int half_width = ceil((float) width / 2);
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < half_width; w++)
        {
            RGBTRIPLE swap = image[h][w];
            image[h][w] = image[h][width - w - 1];
            image[h][width - w - 1] = swap;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE(*original_image)[width] = calloc(height, width * sizeof(RGBTRIPLE));

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            RGBTRIPLE pixel = image[h][w];
            original_image[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = get_box_average(height, width, h, w, image, original_image);
        }
    }
    free(original_image);
    return;
}

RGBTRIPLE get_box_average(int height, int width, int center_h, int center_w, RGBTRIPLE image[height][width],
                          RGBTRIPLE original_image[height][width])
{
    int redSum = 0;
    int greenSum = 0;
    int blueSum = 0;
    int count = 0;
    for (int h = center_h - 1; h <= center_h + 1; h++)
    {
        if (h < 0 || h > height - 1)
        {
            continue;
        }
        for (int w = center_w - 1; w <= center_w + 1; w++)
        {
            if (w < 0 || w > width - 1)
            {
                continue;
            }
            count += 1;
            redSum += original_image[h][w].rgbtRed;
            greenSum += original_image[h][w].rgbtGreen;
            blueSum += original_image[h][w].rgbtBlue;
        }
    }

    RGBTRIPLE average;
    average.rgbtRed = round((float) redSum / count);
    average.rgbtGreen = round((float) greenSum / count);
    average.rgbtBlue = round((float) blueSum / count);
    return average;
}

BYTE clamp_color(int target)
{
    if (target > 255)
    {
        return 255;
    }
    if (target < 0)
    {
        return 0;
    }
    return target;
}
