.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 72.
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 73.
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 74.
# =======================================================
matmul:
    # Error checks
    addi t0, x0, 1
    blt a1, t0, dim_error_1
    blt a2, t0, dim_error_1
    blt a4, t0, dim_error_2
    blt a5, t0, dim_error_2
    bne a2, a4, shape_error

    # Prologue
    addi sp, sp, -36
    sw ra 0(sp)
    sw s0 4(sp)
    sw s1 8(sp)
    sw s2 12(sp)
    sw s3 16(sp)
    sw s4 20(sp)
    sw s5 24(sp)
    sw s6 28(sp)
    sw s7 32(sp)

    add s0, x0, x0 # row counter for m0
    add s1, x0, x0 # col counter for m1
    add s2, x0, a0 # pos of cur m0 row
    add s3, x0, a3 # pos of cur m1 col
    add s4, x0, a1 # the # of rows (height) of m0
    add s5, x0, a2 # the # of columns (width) of m0
    add s6, x0, a5 # the # of columns (width) of m1
    add s7, x0, a6 # the pointer to the the start of d

    slli t0, s5, 2  # offset for m0

outer_loop_start:

    jal inner_loop_start

outer_loop_continue:
    slli t0, s5, 2
    add s2, s2, t0
    slli t1, s6, 2
    sub s3, s3, t1
    addi s0, s0, 1
    add s1, x0, x0
    beq s0, s4, outer_loop_end
    j outer_loop_start


inner_loop_start:

    add a0, s2, x0
    add a1, s3, x0
    add a2, s5, x0
    addi a3, x0, 1
    add a4, s6, x0

    jal dot


    slli t0, s5, 2  # offset for m0

    # dot 返回值就错了

    sw a0, 0(s7)
    addi s7, s7, 4

    addi s1, s1, 1
    addi s3, s3, 4
    beq s1, s6, inner_loop_end
    j inner_loop_start



inner_loop_end:
    j outer_loop_continue




outer_loop_end:

    # Epilogue
    lw ra 0(sp)
    lw s0 4(sp)
    lw s1 8(sp)
    lw s2 12(sp)
    lw s3 16(sp)
    lw s4 20(sp)
    lw s5 24(sp)
    lw s6 28(sp)
    lw s7 32(sp)
    addi sp, sp, 36
    ret

dim_error_1:
    li a0 72
    call exit2

dim_error_2:
    li a0 73
    call exit2

shape_error:
    li a0 74
    call exit2
