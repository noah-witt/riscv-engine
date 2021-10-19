# this program takes input from a user and prints the integer divisors of it.


main:
    PRINT 0, nl
    PRINT 0, welcome_str
    PRINT 0, c_str
    INPUTI temp_input
    LD t0, temp_input
    LUI t1, 0
    BLT t0, t1, exit # if t0 < t1 we jump to the exit
    # TODO print the number and formatting
    PRINT 0, nl
    PRINT 0, intro_str
    PRINT 0, c_str
    LUI t5, 2
    # t2=t0/2+1
    DIV t2, t0, t5
    ADDI t2, t2, 1 #t2 is the divisor
remCalcStart:
    LUI t3, 0
    BGE t3, t2, main # jump back to main if 0>=t2
    REM t4, t0, t2 
    BNE t4, t3, remCalcStartEnd # t3 is zero. 
    # continue here if it is zero
    # print a space then the divisor
    PRINT 0, space_str
    SD t2, temp_output
    PRINT 1, temp_output
remCalcStartEnd:
    ADDI t2, t2, -1
    JAL zero, remCalcStart

exit:
    PRINT 0, exit_str
    HALT

exit_str: .asciiz "bye."
welcome_str: .asciiz "Enter a number to list the divisors or -1 to quit"
intro_str: .asciiz "printing divisors "
c_str: .dword 58
.dword 0
nl: .dword 10
.dword 0
space_str: .asciiz "  "
temp_input: .dword -1
temp_output: .dword 0
