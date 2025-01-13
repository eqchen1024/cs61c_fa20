.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:

    # Prologue
    li t0, 1
    blt a1, t0, no_element_error
    add t0, x0, x0
    add t4, a0, x0
    lw t1, 0(a0)
    add t2, x0, x0


loop_start:
    lw t3, 0(t4)
    bgt t3, t1, update_max
    j loop_continue

update_max:
    add t1, t3, x0
    add t2, t0, x0
    j loop_continue

loop_continue:
    addi t0, t0, 1
    addi t4, t4, 4
    beq t0, a1, loop_end
    j loop_start

no_element_error:
    li a0, 77
    ecall

loop_end:
    addi a0, t2, 0

    # Epilogue

    ret
