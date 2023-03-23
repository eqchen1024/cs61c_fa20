/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

#define MAX_COL_NUM 1200
#define MAX_ROW_NUM 800

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	//YOUR CODE HERE
    // 1. open file
    char format[3];
    Image *img_p = malloc(sizeof(Image));
    FILE *fp = fopen(filename,"r");
    // 2. extract var declare
    uint32_t scale;
    int r;
    int g;
    int b;
    // 3.extract meta data
    fscanf(fp, "%s",format);
    fscanf(fp,"%d%d",&img_p->cols,&img_p->rows);
    fscanf(fp,"%d",&scale);
    // 4. extract rgb data
    img_p->image = (Color **) malloc(sizeof(Color *) * img_p->cols * img_p->rows);
    Color **cur = img_p->image;
    while (fscanf(fp, "%d%d%d",&r,&g,&b) != EOF){
        Color *tmp = (Color *) malloc(sizeof(Color));
        tmp->R = r;
        tmp->G = g;
        tmp->B = b;
        *cur = tmp;
        cur++;
    }
    fclose(fp);
    return img_p;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
    printf("P3\n");
    printf("%d %d\n",image->cols,image->rows);
    printf("255 \n");
    Color **cur = image->image;
    for(int i = 0;i < (image->cols * image->rows);i++){
        printf("%3d %3d %3d",(**(cur)).R,(**(cur)).G,(**(cur)).B);
        if (i % (image->cols) != image->cols -1) {
            printf("   ");
        } else {
            printf("\n");
        }
        cur++;
    }
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
    free(image->image);
    free(image);
}
