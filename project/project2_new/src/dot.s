.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
dot:
    addi t0, x0, 1 # t0 as 1 to check exceptions and counter
    blt a2, t0, length_error
    blt a3, t0, stride_error
    blt a4, t0, stride_error
    # Prologue
    add t1, x0, x0 # hold v0 element and mul
    add t2, x0, x0 # hold v1 element and mul
    add t3, x0, x0 # save sum
    addi t0, x0, 0 # initialize counter for array length
    slli a3, a3, 2 # stride in byte to stride in word
    slli a4, a4, 2 # stride in byte to stride in word

loop_start:
    lw t1 0(a0)
    lw t2 0(a1)
    mul t1, t1, t2
    add t3, t3, t1
    add a0, a0, a3
    add a1, a1, a4
    addi t0, t0, 1
    beq t0, a2, loop_end
    j loop_start

length_error:
    addi a0, x0, 36
    call exit2



stride_error:
    addi a0, x0, 36
    call exit2

loop_end:
   add a0, t3, x0
    # Epilogue

    
    ret
