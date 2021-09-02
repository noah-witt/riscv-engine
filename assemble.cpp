/**
 * @file assemble.cpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @brief compile RISC-V assembly
 * @version 0.1
 * @date 2021-08-31
 * 
 * This is a basic two step compiler. First it identifies symbols
 */

#include <assemble.hpp>
#include <string>

Symbol::Symbol(std::string symbol, unsigned int size) {
    this->symbol = symbol;
    this->size = size;
    this->fixed = false;
    this->referenceLocation = NULL;
}

Symbol::Symbol(std::string symbol, unsigned long address, unsigned int size) {
    this->symbol = symbol;
    this->size = size;
    this->fixed = true;
    this->referenceLocation = address;
}

Symbol::~Symbol() {
    // TODO implement
}

bool Symbol::getFixed() {
    return this->fixed;
}

unsigned int Symbol::getSize() {
    return this->size;
}

std::string Symbol::getSymbol() {
    return this->symbol;
}

bool Symbol::matchesSymbol(std:: string s) {
    return this->symbol == s;
}

unsigned long Symbol::getAddress() {
    return this->referenceLocation;
}


// TODO symbol table
// TODO generate symbol table
// TODO use symbols compile
// TODO load to memory.