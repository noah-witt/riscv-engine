RISC-V is an open-source instruction set architecture. It allows anyone to implement or use it without paying a royalty. This is in contrast with the two most popular alternatives, x86, and ARM. In both ARM and x86, significant royalties must be paid to the designers to implement the architecture. This program provides a robust and straightforward environment to assemble and execute RISC-V assembly code. It implements the vast majority of the userspace RISC-V instruction set, including the multiplication extensions. There are also some additions to enable easier input and output. 

To use the program, you must first write some basic RISC-V assembly. Here is an example of RISC-V assembly.
```
# this program takes input from a user and prints the integer divisors of it.


main:
    PRINT 0, nl
    PRINT 0, welcome_str
    PRINT 0, c_str
    INPUTI temp_input
    LD t0, temp_input
    LUI t1, 0
    BLT t0, t1, exit # if t0 < t1 we jump to the exit
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
```
In order to execute the assembly code run this command `./riscv.run --input ./test_divs.S` where `./test_divs.S` is the filename of the assembly. The `--d` option enables the debugger. The debugger allows you to read memory, and registers, and step through the code. 


Here are some helpful resources for learning RISC-V assembly:
https://mark.theis.site/riscv/
https://web.eecs.utk.edu/~smarz1/courses/ece356/notes/assembly/


# SCOPE
Scope of this project will include an assembler, and the ability to execute the assembled code.

#  Implementation differences
 - Bit patterns are not the same as on real hardware.
 - Some commands have been added that allow for easy input and output

# Added commands

Command | Description | Example
---- | ---- | ---- 
`HALT` | stop execution | `HALT`
`PRINT 0, {memoryAddress}` | Print out a null terminated string stored at `{memoryAddress}` | `PRINT 0, welcome_str`
`PRINT 1,  {memoryAddress}` | Print out an integer that is stored at `{memoryAddress}` | `PRINT 1, out_val`
`INPUTI {memoryAddress}` | Read an integer from standard in and store it at `{memoryAddress}` | `INPUTI in_val`
`INPUTS {memoryAddress}` | Read a string from standard in and store it at `{memoryAddress}` with a null at the end | `INPUTS in_str`
`DEBUG {someRegister}` | print out some register value to the debug output | `DEBUG t0`
