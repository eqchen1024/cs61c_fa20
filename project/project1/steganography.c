/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
    Color **cur = image->image;
    uint32_t pos = row * image->cols + col;
    uint32_t cnt = 0;
    while (cnt < pos) {
        cur++;
        cnt++;
    }
    return *cur;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	//YOUR CODE HERE
    Color **cur = image->image;
    uint32_t total = image->cols * image->rows;
    uint32_t cnt = 0;
    while (cnt < total) {
        if(((*cur)->B &0x1) == 0x1){
            (*cur)->B = 255;
            (*cur)->G = 255;
            (*cur)->R = 255;
        } else {
            (*cur)->B = 0;
            (*cur)->G = 0;
            (*cur)->R = 0;
        }
        cnt++;
        cur++;
    }
    return image;
}


/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/

int main(int argc, char **argv)
{
	//YOUR CODE HERE
    Image *img = readData("./testInputs/JohnConway.ppm");
    Image *new_img = steganography(img);
    writeData(new_img);
    freeImage(new_img);
}
