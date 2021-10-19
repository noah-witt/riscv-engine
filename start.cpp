/**
 * @file start.cpp
 * @author Noah Witt naqitt18@ole.augie.edu
 * @brief Provides management function to control the flow of execution.
 * @version 0.1
 * @date 2021-10-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./start.hpp"
#include "./assemble.hpp"
#include <fstream>
#include <iostream>

/**
 * @brief compile and then execute a RISC-V assembly program
 * 
 * @param fName the file name
 * @param maxSteps 
 * @return int 
 */
int start(std::string fName, int maxSteps) {
    return start(fName, maxSteps, std::cin, std::cout);
}

int start(std::string fName, int maxSteps, std::istream &in, std::ostream &out) {
    std::ifstream ifs(fName);
    Program program = Program(ifs);
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    // next setup stack pointer which is 2 to the top of memory
    a.getReg()->getRegister(2)->write<long>(MAXMEMORY-INSTRUCTION_LENGTH);
    a.loop(in, out, maxSteps);
    return 0;
}
