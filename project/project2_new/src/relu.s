.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
    # Prologue

    addi t3, x0, 1
    blt a1, t3, no_element_error
    add t0, x0, x0
    add t1, x0, a0

loop_start:
    lw t2, 0(t1)
    blt t2, x0, do_neg
    sw t2, 0(t1)
    j loop_continue


loop_continue:
    addi t0, t0, 1
    addi t1, t1, 4
    beq t0, a1, loop_end
    j loop_start

do_neg:
    sw x0, 0(t1)
    j loop_continue

no_element_error:
    li a0, 78
    ecall

loop_end:
    # Epilogue
    
	ret
