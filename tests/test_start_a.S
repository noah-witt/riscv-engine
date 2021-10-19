# this is a simple program that tests many parts of the code

# test some basic math
LW t0, startVal
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
ADDI t0, t0, -64
LUI t1, 67
SW t1, t0 
ADDI t0, t0, -64
LUI t1, 66
SW t1, t0 
ADDI t0, t0, -64
LUI t1, 65
SW t1, t0 
PRINT 0, t0
HALT




# data
second_val: .dword 775
startVal: .dword 195
resultVal: .dword 0
space: .asciiz " "


# jalr test
sect_b:
    LD t0, second_val
    JALR zero, ra, 0
