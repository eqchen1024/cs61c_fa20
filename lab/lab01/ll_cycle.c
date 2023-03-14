#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    /* your code here */
    if (head == NULL) {
        return 0;
    }
    node *fast = head;
    node *slow = head;
    while (fast->next != NULL && fast->next->next != NULL) {
        fast = fast->next ->next;
        slow = slow->next;
        if (fast == slow) {
            return 1;
        }
    }
    return 0;
}