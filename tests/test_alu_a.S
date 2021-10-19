# example test script
HALT
# this code contains references so we can easily run parts of this script
JAL zero, sectA
JAL zero, sectB
JAL zero, sectC
JAL zero, setC_d
JAL zero, sectD_lt
JAL zero, sectD_eq
JAL zero, sectD_gt
JAL zero, sectE
HALT

#this section is testing basic running.
sectA: 
    ADDI t0, zero, 150
    ADDI t1, zero, 550
    ADD t2, t0, t1
    HALT
# section b is designed to test
sectB:
    LW t0,sectB_a
    LW t1,sectB_b
    ADD t2, t0, t1
    MV a0, t2
    NEG t2, t2
    MV a1, t2
    HALT
sectB_a: .word 550
sectB_b: .word 250
# section c is a test of the branching instructions
sectC:
    NOP
    ADDI a3, zero, 980
    ADDI t0, zero, 15
    ADDI t1, zero, 30
sectC_a:
    BEQ t0, t1, sectC_b
    ADDI a3, zero, 990
    HALT
sectC_b: ADDI a3, zero, 991
    HALT
setC_d:
    ADDI a3, zero, 980
    ADDI t0, zero, 45
    ADDI t1, zero, 45
    JAL zero, sectC_a
    HALT

sectD:
sectD_lt: 
    ADDI t0, zero, 795
    ADDI t1, zero, 995
    JAL, zero, sectD_go
sectD_eq:
    ADDI t0, zero, 1025
    ADDI t1, zero, 1025
    JAL, zero, sectD_go
    HALT
sectD_gt:
    ADDI t0, zero, 1095
    ADDI t1, zero, 1005
    JAL, zero, sectD_go
    HALT
sectD_go:
    BLT t0, t1, sectD_j
    ADDI a3, zero, 200
    HALT
sectD_j:
    ADDI a3, zero, 2001
    HALT
sectD_a: .asciiz "abc123"


# this section marks where some more complicated scripts are tested

sectE:
    PRINT 0 sectE_a
    INPUTS sectE_a
    PRINT 0 sectE_a
    HALT
sectE_a: .asciiz "demo output abc123"
sectE_b: .dword 150 

sectF:
    
