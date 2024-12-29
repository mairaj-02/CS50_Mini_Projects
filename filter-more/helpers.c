#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
  for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float colour = 0.0;
            colour = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0); //calculating average
            image[i][j].rgbtBlue = colour;
            image[i][j].rgbtGreen = colour; //assigning new values after rounding off
            image[i][j].rgbtRed = colour;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
        for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            // Check all surrounding pixels including the pixel itself
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if the surrounding pixel is within the bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sumRed += image[ni][nj].rgbtRed;
                        sumGreen += image[ni][nj].rgbtGreen;
                        sumBlue += image[ni][nj].rgbtBlue;
                        count++;
                    }
                }
            }

            // Calculate the average color values
            temp[i][j].rgbtRed = round((float) sumRed / count);
            temp[i][j].rgbtGreen = round((float) sumGreen / count);
            temp[i][j].rgbtBlue = round((float) sumBlue / count);
        }
    }

    // Copy the blurred pixels back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
        int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int rgx = 0, ggx = 0, bgx = 0, rgy = 0, ggy = 0, bgy = 0;
            float rg = 0, gg = 0, bg = 0;

          // for every block
            for (int di = -1; di <= 1; di++)
            {
                for (int dj = -1; dj <= 1; dj++)
                {
                    int ni = i + di;
                    int nj = j + dj;

                    // Check if the surrounding pixel is within the bounds
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        rgx += (image[ni][nj].rgbtRed * Gx[di + 1][dj +1]);
                        ggx += (image[ni][nj].rgbtGreen * Gx[di + 1][dj +1]);
                        bgx += (image[ni][nj].rgbtBlue * Gx[di + 1][dj +1]);
                        rgy += (image[ni][nj].rgbtRed * Gy[di + 1][dj +1]);
                        ggy += (image[ni][nj].rgbtGreen * Gy[di + 1][dj +1]);
                        bgy += (image[ni][nj].rgbtBlue * Gy[di + 1][dj +1]);
                    }
                }
            }
            rg = pow(rgx, 2) + pow(rgy, 2);
            rg = round(pow(rg, 0.5));
            if (rg > 255)
            {rg = 255;}
            gg = pow(ggx, 2) + pow(ggy, 2);
            gg = round(pow(gg, 0.5));
            if (gg > 255)
            {gg = 255;}
            bg = pow(bgx, 2) + pow(bgy, 2);
            bg = round(pow(bg, 0.5));
            if (bg > 255)
            {bg = 255;}
            // Calculate the average color values
            temp[i][j].rgbtRed = rg;
            temp[i][j].rgbtGreen = gg;
            temp[i][j].rgbtBlue = bg;
        }
    }

    // Copy the edges pixels back to the original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    return;
}