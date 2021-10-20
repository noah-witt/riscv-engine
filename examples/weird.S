
PRINT 0, welcome # print a welcome message
INPUTI sp # take input and store at the sp location
LD a0, sp # load the input intiger in to a0
ADDI sp, sp -8 # move the stack pointer for the function call
JAL ra, work # call the work fucntion
ADDI sp, sp 8 # move the stack pointer back after the function call
PRINT 0, newline # print a new line
HALT

# work iterates through the numbers of the weird algo
# parameters int a0
work:
    # print the number
    SD a0, temp # store the a0 value in the temporary memory location for it to be printed
    PRINT 0, space # print a space
    PRINT 1, temp # print the a0 value from temperary
    LUI t0, 2 # put 2 in t0
    BLT a0, t0, work_return # if a0<2 jump to work_return
    LUI t0, 2 # put 2 in t0
    REM t0, a0, t0 # t0=a0%2
    LUI t1, 0 # t1=0
    BEQ t0, t1, work_even # if t0==t1 jump to work_even
    # odd section
work_odd:
    LUI t0, 3 # t0 = 3
    # a0 = (a0*3)+1
    MUL a0, a0, t0 # a0=a0*3
    ADDI a0, a0, 1 # a0=a0+1
    # store the return address before recursively calling work
    SD ra, sp # store the return address 
    ADDI sp, sp, -8 # move the stack pointer for the recursive call
    JAL ra, work
    ADDI sp, sp, 8 # set the stack pointer back
    LD ra, sp # load the ra
work_even:
    LUI t0, 2
    DIV a0, a0, t0
    # store the return address before recursively calling work
    SD ra, sp # store the return address 
    ADDI sp, sp, -8 # move the stack pointer for the recursive call
    JAL ra, work
    ADDI sp, sp, 8 # set the stack pointer back
    LD ra, sp # load the ra
work_return:
    JALR zero, ra, 0 # return from this function call


# constants
welcome: .asciiz "Enter an integer "
newline: .dword 10
.dword 0
space: .asciiz " "
temp: .dword 0
