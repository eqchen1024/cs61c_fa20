.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 89.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 90.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 92.
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp, -32
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw ra, 28(sp)

	add s0, a0, x0 # file path
	add s1, a1, x0
	add s2, a2, x0

    # open file
    add a1, s0, x0 # pass args
    add a2, x0, x0  # pass args
    jal fopen # call func
    add s4, x0, a0 # descriptor
    addi t0, x0, -1 # error code
    beq s4, t0, fopen_error

    # alloc space in mem
    addi a0, x0, 8 # pass args
    jal malloc # call func
    beq a0, x0, malloc_error
    add s6, x0, a0 # pointer to the allocated space

    # read file get row and col
    add a1, s4, x0 # pass args
    add a2, s6, x0 # pass args
    addi a3, x0, 8 # pass args
    jal fread # call func
    addi t1, x0, 8
    bne a0, t1, fread_error
    lw t0, 0(s6) # num of rows
	lw t1, 4(s6) # num of cols

	mul s5, t0, t1 # num of elements of m
    slli s5, s5, 2 # num of bytes of m


    sw t0, 0(s1)
    sw t1, 0(s2)

    # alloc space in mem
    add a0, s5, x0 # pass args
    jal malloc # call func
    beq a0, x0, malloc_error
    add s3, x0, a0 # pointer to the allocated space


    # read file and get element
    add a1, s4, x0 # pass args
    add a2, s3, x0 # pass args
    add a3, s5, x0 # pass args
    jal fread # call func
    bne a0, s5, fread_error

    # close file
    add a1, s4, x0 # pass args
    jal fclose # call func
    bne a0, x0, fclose_error


    add a0, x0, s3
    # Epilogue

    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw ra, 28(sp)
    addi sp, sp, 32

    ret

malloc_error:
    addi a1, x0, 89
    call exit2

fopen_error:
    addi a1, x0, 90
    call exit2

fread_error:
    addi a1, x0, 91
    call exit2

fclose_error:
    addi a1, x0, 92
    call exit2