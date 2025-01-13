.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 93.
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 94.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 95.
# ==============================================================================
write_matrix:

    # Prologue
    addi sp, sp, -24
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw ra, 20(sp)

    # save args
    add s0, a1, x0 # address of m0
    add s1, a2, x0 # num rows
    add s2, a3, x0 # num cols

    # open file
    add a1, a0, x0
    addi a2, x0, 1
    jal fopen
    addi t0, x0, -1
    beq a0, t0, fopen_error
    add s3, a0, x0 # descriptor

    # malloc space for row col
    addi a0, x0, 8
    jal malloc
    beq a0, x0, malloc_error
    add s4, a0, x0

    # write row col to heap
    sw s1, 0(s4)
    sw s2, 4(s4)


    # write file row col
    add a1, s3, x0
    add a2, s4, x0
    addi a3, x0, 2
    addi a4, x0, 4
    jal fwrite
    addi t0, x0, 2
    bne a0, t0, fwrite_error

    # write file m0
    mul t0, s1, s2
    add a1, s3, x0
    add a2, s0, x0
    add a3, t0, x0
    addi a4, x0, 4
    jal fwrite
    mul t0, s1, s2
    bne a0, t0, fwrite_error




    # file close
    add a1, s3, x0
    jal fclose
    bne a0, x0, fclose_error


    # Epilogue

    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw ra, 20(sp)
    addi sp, sp, 24


    ret

fopen_error:
    addi a1, x0, 93
    call exit2

fwrite_error:
    addi a1, x0, 94
    call exit2

fclose_error:
    addi a1, x0, 95
    call exit2

malloc_error:
    addi a1, x0, 96
    call exit2