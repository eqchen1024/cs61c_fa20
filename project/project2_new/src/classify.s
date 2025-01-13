.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero,
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminats the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    # check arg num
    addi t0, x0, 5
    bne a0, t0, argc_error

    # save reg
    addi sp, sp, -36
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw ra, 32(sp)

    # save print stat
    addi sp, sp, -4
    sw a2, 0(sp)

    # save args
    lw s0, 4(a1) # m0 path
    lw s1, 8(a1) # m1 path
    lw s2, 12(a1) # input path
    lw t1, 16(a1) # output path

    # save output path for further use
    addi sp, sp, -4
    sw t1, 0(sp)

	# =====================================
    # LOAD MATRICES
    # =====================================

    # load input
    # malloc space for addr of row and col
    addi a0, x0, 24
    jal malloc
    beq a0, x0, malloc_error
    add t2, a0, x0

    addi sp, sp, -4
    sw t2, 0(sp)

    add a0, s2, x0
    add a1, t2, x0
    addi a2, t2, 4
    jal read_matrix
    lw t2, 0(sp)
    add s2, a0, x0 # addr of input
    lw s7, 0(t2) # row of input
    lw s4, 4(t2) # col of input



    # load m0
    add a0 , s0, x0
    addi a1, t2, 8
    addi a2, t2, 12
    jal read_matrix
    lw t2, 0(sp)
    add s0, a0, x0 # addr of m0
    lw s5, 8(t2) # row of m0



    # load m1
    add a0 , s1, x0
    addi a1, t2, 16
    addi a2, t2, 20
    jal read_matrix
    lw t2, 0(sp)
    add s1, a0, x0 # addr of m1
    lw s6, 16(t2) # row of m1

    addi sp, sp, 4

    # free 24 bytes for row and col
    add a0, t2, x0
    jal free


    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input

    # malloc layer1
    mul a0, s5, s4
    slli a0, a0, 2
    jal malloc
    beq a0, x0, malloc_error
    add s3, a0, x0 # addr layer1


    # mul layer1
    add a0, s0, x0
    add a1, s5, x0
    add a2, s7, x0
    add a3, s2, x0
    add a4, s7, x0
    add a5, s4, x0
    add a6, s3, x0
    jal matmul

    # free input and m0
    add a0, s2, x0
    jal free
    add a0, s0, x0
    jal free

    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    add a0, s3, x0
    mul a1, s5, s4
    jal relu


    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)
    # malloc linear layer
    mul a0, s6, s4
    slli a0, a0, 2
    jal malloc
    beq a0, x0, malloc_error
    add s0, a0, x0 # addr of linear layer

    # mul linear layer
    add a0, s1, x0
    add a1, s6, x0
    add a2, s5, x0
    add a3, s3, x0
    add a4, s5, x0
    add a5, s4, x0
    add a6, s0, x0
    jal matmul

    # free m1 and prev layer
    add a0, s1, x0
    jal free
    add a0, s3, x0
    jal free


    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix
    lw t1, 0(sp) # output path
    addi sp, sp, 4

    add a0, t1, x0
    add a1, s0, x0
    add a2, s6, x0
    add a3, s4, x0
    jal write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax
    add a0, s0, x0
    mul a1, s6, s4
    jal argmax
    add s1, a0, x0

    # free linear layer
    add a0, s0, x0
    jal free

    # Print classification
    lw a2, 0(sp)
    addi sp, sp, 4
    beq a2, x0, do_print
    j end

end:

    # Print newline afterwards for clarity
    # don't know how to do


    # restore
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw ra, 32(sp)
    addi sp, sp, 36


    ret

argc_error:
    addi a1, x0, 89
    call exit2

malloc_error:
    addi a1, x0, 88
    call exit2

do_print:
    add a1, s1, x0
    jal print_int
    j end