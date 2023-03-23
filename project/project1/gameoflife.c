/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.

Color *evaluateOnePixel(Image *image, int row, int col)
{
    Color **cur = image->image;
    uint32_t pos = row * image->cols + col;
    uint32_t cnt = 0;
    Color *res = malloc(sizeof(Color));
    while (cnt < pos) {
        cur++;
        cnt++;
    }
    res -> B = (*cur) -> B;
    res -> G = (*cur) -> G;
    res -> R = (*cur) -> R;
    return res;
}

Color **get_neighbours(Image *image, int row, int col) {
    Color** nb = malloc(sizeof(Color *) * 8); // bug 1. forget to time num of elements.
    int up_row_idx = (row - 1 + image->rows) % image->rows;
    int down_row_idx = (row + 1) % image->rows;
    int left_col_idx = (col - 1 + image->cols) % image->cols;
    int right_col_idx = (col + 1) % image->cols;
    nb[0] = evaluateOnePixel(image,up_row_idx,col);
    nb[1] = evaluateOnePixel(image,down_row_idx,col);
    nb[2] = evaluateOnePixel(image,row,left_col_idx);
    nb[3] = evaluateOnePixel(image,row,right_col_idx);
    nb[4] = evaluateOnePixel(image,up_row_idx,left_col_idx);
    nb[5] = evaluateOnePixel(image,up_row_idx,right_col_idx);
    nb[6] = evaluateOnePixel(image,down_row_idx,left_col_idx);
    nb[7] = evaluateOnePixel(image,down_row_idx,right_col_idx);
    return nb;
}

int get_pos_bit(int num, int pos) {
    int mask = 1 << pos;
    return (num & mask) >> pos;
}

int get_channel(Color *cr_p,char flag){
    if (flag == 'r'){
        return cr_p->R;
    } else if (flag == 'g'){
        return cr_p->G;
    } else {
        return cr_p->B;
    }
}

int get_num_of_ones(Color **nb,int pos,char channel) {
    int cnt = 0;
    for (int i = 0; i < 8; i++) {
        cnt += get_pos_bit(get_channel(nb[i],channel),pos);
    }
    return cnt;
}
//  bug2 问题出在这里 判断条件错
int live_status_check(int cur_bit, int bit_sum, int rule) {
    if ((cur_bit == 1) && (get_pos_bit(rule,bit_sum + 9) == 1)) {
        return 1;
    } else if ((cur_bit == 0) && (get_pos_bit(rule,bit_sum) == 1)) {
        return 1;
    } else {
            return 0;
    }
}
// 测到这里
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule) {
	//YOUR CODE HERE
    Color *next_gen = malloc(sizeof(Color));
    Color *cur = evaluateOnePixel(image,row,col);
    int cur_r = get_channel(cur,'r');
    int cur_g = get_channel(cur,'g');
    int cur_b = get_channel(cur,'b');

    Color **nb = get_neighbours(image,row,col);
    // for bits
    int r_res = 0;
    int g_res = 0;
    int b_res = 0;
    int base = 1;
    for (int i = 0; i < 8; i++) {
        int r_bit = get_pos_bit(cur_r,i);
        int g_bit = get_pos_bit(cur_g,i);
        int b_bit = get_pos_bit(cur_b,i);
        int r_cnt = get_num_of_ones(nb,i,'r');
        int g_cnt = get_num_of_ones(nb,i,'g');
        int b_cnt = get_num_of_ones(nb,i,'b');

        int next_gen_bit_r = live_status_check(r_bit,r_cnt,rule);
        int next_gen_bit_g = live_status_check(g_bit,g_cnt,rule);
        int next_gen_bit_b = live_status_check(b_bit,b_cnt,rule);
        // 测到这里 bug 3 反了
        r_res = r_res + next_gen_bit_r * base;
        g_res = g_res + next_gen_bit_g * base;
        b_res = b_res + next_gen_bit_b * base;
        base *= 2;
    }
    next_gen->R = r_res;
    next_gen->G = g_res;
    next_gen->B = b_res;
    return next_gen;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
    Image *next_gen = malloc(sizeof(Image));
    next_gen->cols = image->cols;
    next_gen->rows = image->rows;
    next_gen->image = malloc(sizeof(Color) * image->cols * image->rows);
	//YOUR CODE HERE
    Color ** cur = next_gen->image;
    for(int i = 0; i < image->cols * image->rows;i++) {
        int row = i /image->cols;
        int col = i % image->cols;
        *cur = evaluateOneCell(image,row,col,rule);
        cur++;
    }
    return next_gen;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
    if (argc != 3) {
        return -1;
    }
    char *ptr;
    Image *img = life(readData(argv[1]), strtol(argv[2],&ptr,16));
    writeData(img);
    freeImage(img);
}