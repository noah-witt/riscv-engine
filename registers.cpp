/**
 * @file registers.cpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @brief Provides registers
 * @version 0.1
 * @date 2021-08-05
 * 
 * 
 */

#include "./registers.hpp"
#include "./registers.t.hpp"
#include "./memory.hpp"
#include <stdlib.h>
#include <unordered_map>
#include <type_traits>
#include <array>

Register::Register() {
    this->value = (unsigned long *)calloc(1, sizeof(unsigned long));
    this->isMutable = true;
}

Register::~Register() {
    // free(this->value);
}


zeroRegister::zeroRegister() {
    this->value = (unsigned long *)calloc(1, sizeof(unsigned long));
    this->isMutable = false;
}


Registers::Registers() {
    this->registers = std::unordered_map<unsigned int, Register>();
    this->registers.insert(std::pair<unsigned int, Register>(0, zeroRegister()));
    for(unsigned int i=1; i<=REGISTERS_END; i++) {
        //create registers for [1, REGISTERS_END]
        this->registers.insert(std::pair<unsigned int, Register>(i, Register()));
    }
    this->registers.insert(std::pair<unsigned int, Register>(PC, Register()));
    /**
     * We now have registers in the range 0-64.
     * the program counter is at 200
     */
}

Registers::~Registers() {
    std::unordered_map<unsigned int, Register>::iterator it = this->registers.begin();
    while(it != this->registers.end()) {
        it->second.~Register();
    }
    this->registers.~unordered_map();
}

Register *Registers::getRegister(unsigned int x) {
    std::unordered_map<unsigned int, Register>::iterator items = this->registers.find(x);
    return &(items->second);
}
