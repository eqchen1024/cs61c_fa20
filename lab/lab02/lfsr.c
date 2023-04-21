#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lfsr.h"

void lfsr_calculate(uint16_t *reg) {
    /* YOUR CODE HERE */
    uint16_t bit_at_0, bit_at_2, bit_at_3, bit_at_5;
    bit_at_0 = ((*reg) >> 0) & 1;
    bit_at_2 = ((*reg) >> 2) & 1;
    bit_at_3 = ((*reg) >> 3) & 1;
    bit_at_5 = ((*reg) >> 5) & 1;
    uint16_t bit_shift_in  = (((bit_at_0 ^ bit_at_2) ^ bit_at_3) ^ bit_at_5);
    *reg = ((*reg >> 1) & ~(1 << 15)) | (bit_shift_in << 15);
}

