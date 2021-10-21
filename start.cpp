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
#include <boost/log/trivial.hpp>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <vector>
#include "./util.hpp"
#include "./assemble.hpp"
#include <iostream>

/**
 * @brief compile and then execute a RISC-V assembly program
 * 
 * @param fName the file name
 * @param maxSteps 
 * @return int 
 */
int start(std::string fName, int maxSteps, bool debugMode) {
    return start(fName, maxSteps, std::cin, std::cout, debugMode);
}

int start(std::string fName, int maxSteps, std::istream &in, std::ostream &out, bool debugMode) {
    std::ifstream ifs(fName);
    if(ifs.fail()) {
        BOOST_LOG_TRIVIAL(warning) << "file read error";
        return 1;
    }
    Program program = Program(ifs);
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    // next setup stack pointer which is 2 to the top of memory
    a.getReg()->getRegister(2)->write<long>(MAXMEMORY-INSTRUCTION_LENGTH);
    a.loop(in, out, maxSteps, debugMode);
    return 0;
}
