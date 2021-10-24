init:
    PRINT 0, welcome_str
    ADDI sp, sp, -8
    INPUTI sp
    LD a0, sp
    LD a1, a_char
    LD a2, c_char
    LD a3, b_char
    JAL ra, towers
    PRINT 0, newline
    PRINT 0, end_str
    PRINT 0, newline
    HALT

welcome_str:
    .asciiz "Enter the number of disks "
end_str:
    .asciiz "done"
a_char: .dword 'A'
b_char: .dword 'B'
c_char: .dword 'C'
newline: 
    .dword 10
    .dword 0

# a0=num, a1=frompeg, a2=topeg, a3=auxpeg, a4=scratch
towers:
    LUI t0, 1
    BEQ t0, a0, num_1_case
    # recursive call one
    sd a0, sp
    sd a1, (-8)sp
    sd a2, (-16)sp
    sd a3, (-24)sp
    sd ra, (-32)sp
    addi a0, a0, -1
    # swap a3 and a2
    mv a4, a3
    mv a3, a2
    mv a2, a4
    addi sp, sp -40
    JAL ra, towers
    addi sp, sp 40
    ld a0, sp
    ld a1, (-8)sp
    ld a2, (-16)sp
    ld a3, (-24)sp
    ld ra, (-32)sp
    # print out this move
    sd ra, sp
    addi sp, sp, -8
    JAL ra, print_mov
    addi sp, sp, 8
    ld ra, sp
    # recurisve call 2
    sd a0, sp
    sd a1, (-8)sp
    sd a2, (-16)sp
    sd a3, (-24)sp
    sd ra, (-32)sp
    addi a0, a0, -1
    # swap a3 and a1
    mv a4, a1
    mv a1, a3
    mv a3, a4
    addi sp, sp -40
    JAL ra, towers
    addi sp, sp 40
    ld a0, sp
    ld a1, (-8)sp
    ld a2, (-16)sp
    ld a3, (-24)sp
    ld ra, (-32)sp
    # return
    jalr zero, ra, 0
    
num_1_case:
    sd ra, sp
    addi sp, sp -8
    JAL ra, print_mov
    addi sp, sp 8
    ld ra, sp
    JALR zero, ra, 0

# a0=num, a1=frompeg, a2=topeg
print_mov:
    PRINT 0, newline
    PRINT 0, mov_disk_str
    SD a0, num_print
    PRINT 1, num_print
    PRINT 0, from_peg_str
    SD a1, char_print
    PRINT 0, char_print
    PRINT 0, to_peg_str
    SD a2, char_print
    PRINT 0, char_print
    JALR, zero, ra, 0

num_print:
    .dword 0

char_print:
    .dword 0
    .dword 0

mov_disk_str:
    .asciiz "Move disk "
from_peg_str:
    .asciiz " from peg "
to_peg_str:
    .asciiz " to peg "