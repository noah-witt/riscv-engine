# this is a simple program that tests many parts of the code

# test some basic math
LW t0, startVal # load the starting value in to memory
ADDI t0, t0, 10
# t0 = 205
ADDI t1, zero, -100
ADD t0, t0, t1
# t0 = 105
SW t0, resultVal
PRINT 1, resultVal
PRINT 0, space
# test JALR
LUI t0, sect_b
JALR ra, t0, 0
SW t0, resultVal
PRINT 1, resultVal
PRINT 0, space
# next is the stack pointer test
# writing a 3 character string to the stack
ADDI t0, sp, 0
LUI t1, 0
SW t1, t0 
ADDI t0, t0, -8
LUI t1, 67
SW t1, t0 
ADDI t0, t0, -8
LUI t1, 66  
SW t1, t0 
ADDI t0, t0, -8
LUI t1, 65
SW t1, t0 
PRINT 0, t0
PRINT 0, space
LUI t0, 15
LUI t1, 5
DEBUG t0
DEBUG t1
DIV t0, t0, t1
SD t0, sp
PRINT 1, sp
ADDI sp, sp, -8
PRINT 0, space
JAL ra, lt_test
PRINT 0, space
PRINT 1, (8)sp
PRINT 0, space # print out a space; this is trailing so the tests need to be written carefully

HALT




# data
second_val: .dword 775
startVal: .dword 195
resultVal: .dword 0
space: .asciiz " "
abc_str: .asciiz "abc" # a string to print out
bca_str: .asciiz "bca


# jalr test
sect_b:
    LD t0, second_val
    JALR zero, ra, 0

# end dump out is a place to go when something takes the "wrong" path for our tests
end_dump_out:
    HALT

lt_test:
    LUI a1, 175
    LUI a0, 150
    BLT a1, a1, end_dump_out
    BLT a1, a0, end_dump_out
    ADDI sp, sp, -8
    LUI a0, 976
    SD a0, sp
    PRINT 1, sp
    ADDI sp, sp 8
    JALR zero, ra, 0