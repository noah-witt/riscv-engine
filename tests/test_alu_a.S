# example test script
HALT
# this code contains references so we can easily run parts of this script
JAL zero, sectA
JAL zero, sectB
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
sectC:
    NOP
    BEQ
