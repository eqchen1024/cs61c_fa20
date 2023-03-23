#include <stdio.h>
int get_pos_bit(int num, int pos) {
    int mask = 1 << pos;
    return (num & mask) >> pos;
}

int live_status_check(int cur_bit, int bit_sum, int rule) {
    if ((cur_bit == 1) && (get_pos_bit(rule,bit_sum) == 1)) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    printf("%d\n",live_status_check(0,1,0x1808));
    printf("%d\n",get_pos_bit(11,1));
    printf("%d\n",get_pos_bit(11,2));
    printf("%d\n",get_pos_bit(11,3));
    printf("%d\n",get_pos_bit(11,4));
}