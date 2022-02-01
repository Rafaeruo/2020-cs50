#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE current;
    int average;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            current = image[i][j];
            average = (int) round((float)(current.rgbtRed + current.rgbtGreen + current.rgbtBlue) / 3);
            current.rgbtRed = average;
            current.rgbtGreen = average;
            current.rgbtBlue = average;
            image[i][j] = current;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int left_wall = 0;
    int right_wall = width - 1;
    RGBTRIPLE temp;
    
    while (left_wall < right_wall)
    {
        
        for (int i = 0; i < height; i++)
        {
            temp = image[i][left_wall];
            image[i][left_wall] = image[i][right_wall];
            image[i][right_wall] = temp;
        }
        
        left_wall++;
        right_wall--;
    }
    
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int starty;
    int startx;
    int endy;
    int endx;
    int startx_save;
    
    int counter;
    int sum_r;
    int sum_g;
    int sum_b;
    
    RGBTRIPLE newimage[height][width];
    
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            starty = i - 1;
            if (i == 0)
            {
                starty = i;
            }
            startx = j - 1;
            if (j == 0)
            {
                startx = j;
            }
            startx_save = startx;
            endy = i + 1;
            if (i == height - 1)
            {
                endy = i;
            }
            endx = j + 1;
            if (j == width - 1)
            {
                endx = j;
            }
            
            counter = 0;
            sum_r =  0;
            sum_g = 0;
            sum_b = 0;
            while (starty <= endy)
            {
                while (startx <= endx)
                {
                    sum_r += image[starty][startx].rgbtRed; 
                    sum_g += image[starty][startx].rgbtGreen;
                    sum_b += image[starty][startx].rgbtBlue;
                    startx ++;
                    counter++;
                }
                
                startx = startx_save;
                starty++;
            }
            
            
            newimage[i][j].rgbtRed = (int) round((float)sum_r / counter);
            newimage[i][j].rgbtGreen = (int) round((float)sum_g / counter);
            newimage[i][j].rgbtBlue = (int) round((float)sum_b / counter);
            
            
        }
    }
    
    //copy newimage pixels to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newimage[i][j];
        }
    }
    
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int calc_r;
    int calc_g;
    int calc_b;
    
    int gx_rgb[3];
    int gy_rgb[3];
    int gx;
    int gy;
    
    RGBTRIPLE newimage[height][width];
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //restoring rgb sums to zero
            gx_rgb[0] = 0;
            gx_rgb[1] = 0;
            gx_rgb[2] = 0;
            gy_rgb[0] = 0;
            gy_rgb[1] = 0;
            gy_rgb[2] = 0;
            
            
            //gx
            //left column (negative)
            if (j != 0)
            {
                gx_rgb[0] += image[i][j - 1].rgbtRed * -2;
                gx_rgb[1] += image[i][j - 1].rgbtGreen * -2;
                gx_rgb[2] += image[i][j - 1].rgbtBlue * -2;
                if (i != 0)
                {
                    gx_rgb[0] += image[i - 1][j - 1].rgbtRed * -1;
                    gx_rgb[1] += image[i - 1][j - 1].rgbtGreen * -1;
                    gx_rgb[2] += image[i - 1][j - 1].rgbtBlue * -1;
                }
                if (i != height - 1)
                {
                    gx_rgb[0] += image[i + 1][j - 1].rgbtRed * -1;
                    gx_rgb[1] += image[i + 1][j - 1].rgbtGreen * -1;
                    gx_rgb[2] += image[i + 1][j - 1].rgbtBlue * -1;
                }
                
            }
            
            //right column (positive)
            if (j != width - 1)
            {
                gx_rgb[0] += image[i][j + 1].rgbtRed * 2;
                gx_rgb[1] += image[i][j + 1].rgbtGreen * 2;
                gx_rgb[2] += image[i][j + 1].rgbtBlue * 2;
                
                if (i != 0)
                {
                    gx_rgb[0] += image[i - 1][j + 1].rgbtRed * 1;
                    gx_rgb[1] += image[i - 1][j + 1].rgbtGreen * 1;
                    gx_rgb[2] += image[i - 1][j + 1].rgbtBlue * 1;
                }
                if (i != height - 1)
                {
                    gx_rgb[0] += image[i + 1][j + 1].rgbtRed * 1;
                    gx_rgb[1] += image[i + 1][j + 1].rgbtGreen * 1;
                    gx_rgb[2] += image[i + 1][j + 1].rgbtBlue * 1;
                }
            }
            
            
            
            //gy
            //top row (negative)
            if (i != 0)
            {
                gy_rgb[0] += image[i - 1][j].rgbtRed * -2;
                gy_rgb[1] += image[i - 1][j].rgbtGreen * -2;
                gy_rgb[2] += image[i - 1][j].rgbtBlue * -2;
                
                if (j != 0)
                {
                    gy_rgb[0] += image[i - 1][j - 1].rgbtRed * -1;
                    gy_rgb[1] += image[i - 1][j - 1].rgbtGreen * -1;
                    gy_rgb[2] += image[i - 1][j - 1].rgbtBlue * -1;
                }
                if (j != width - 1)
                {
                    gy_rgb[0] += image[i - 1][j + 1].rgbtRed * -1;
                    gy_rgb[1] += image[i - 1][j + 1].rgbtGreen * -1;
                    gy_rgb[2] += image[i - 1][j + 1].rgbtBlue * -1;
                }
            }
            
            //bottom row (positive)
            if (i != height - 1)
            {
                gy_rgb[0] += image[i + 1][j].rgbtRed * 2;
                gy_rgb[1] += image[i + 1][j].rgbtGreen * 2;
                gy_rgb[2] += image[i + 1][j].rgbtBlue * 2;
                
                if (j != 0)
                {
                    gy_rgb[0] += image[i + 1][j - 1].rgbtRed * 1;
                    gy_rgb[1] += image[i + 1][j - 1].rgbtGreen * 1;
                    gy_rgb[2] += image[i + 1][j - 1].rgbtBlue * 1;
                }
                if (j != width - 1)
                {
                    gy_rgb[0] += image[i + 1][j + 1].rgbtRed * 1;
                    gy_rgb[1] += image[i + 1][j + 1].rgbtGreen * 1;
                    gy_rgb[2] += image[i + 1][j + 1].rgbtBlue * 1;
                }
            }
            
            //calculating value from gx and gy and capping it at 255
            calc_r = (int) round(sqrt((pow(gx_rgb[0], 2)) + (pow(gy_rgb[0], 2))));
            if (calc_r > 255)
            { 
                calc_r = 255; 
            }
            calc_g = (int) round(sqrt((pow(gx_rgb[1], 2)) + (pow(gy_rgb[1], 2))));
            if (calc_g > 255)
            { 
                calc_g = 255;
            }
            calc_b = (int) round(sqrt((pow(gx_rgb[2], 2)) + (pow(gy_rgb[2], 2))));
            if (calc_b > 255)
            { 
                calc_b = 255;
            }
            
            newimage[i][j].rgbtRed = calc_r;
            newimage[i][j].rgbtGreen = calc_g;
            newimage[i][j].rgbtBlue = calc_b;
            
            
            
        }
    }
    
    //copy newimage pixels to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = newimage[i][j];
        }
    }
    
    
    return;
}
