JAL zero, main


# the return address is stored in ra
# a0 is the inst
# a1 is set to zero if true, and 1 if false.
even_or_odd:
    ADDI a1, zero, 2
    REM a1, a0, a1
    JALR zero, ra, 0

# this prints if a number is even or odd
main:
    PRINT 0, welcome_str
    INPUTI input_val
    LD t1, input_val
    ADDI t0, zero, -1
    BEQ t0, t1, halt_ops
    ADD a0, t1, zero
    JAL ra, even_or_odd
    # a1 is zero or 1
    # ADDI t2, zero, 1
    LW t2, one_val
    ADDI t2, zero, 1
    DEBUG t2,
    DEBUG a1
    BEQ t2, a1, odd
    # even line
    PRINT 0, even_str
    PRINT 0, newline_str
    JAL zero, main
odd:
    PRINT 0, odd_str
    PRINT 0, newline_str
    JAL zero, main
halt_ops:
    PRINT 0, done_str
    HALT
input_val: .word -1
welcome_str: .asciiz "Enter a number of -1 if done "
done_str: .asciiz "bye"
even_str: .asciiz "even"
odd_str: .asciiz "odd"
one_val: .dword 1
newline_str:
    .word 10
    .word 0


