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



zeroRegister::zeroRegister() {
    this->value = (unsigned long *)calloc(1, sizeof(unsigned long));
    this->isMutable = false;
}


Registers::Registers() {
    this->registers = std::unordered_map<unsigned int, Register>();
    this->registers.insert(std::pair<unsigned int, Register>(0, zeroRegister()));
    for(int i=1; i<=REGISTERS_END; i+=1) {
        //create registers for [1, REGISTERS_END]
        //BOOST_LOG_TRIVIAL(debug) << "creating register groupings "<<i;
        this->registers.insert(std::pair<unsigned int, Register>(i, Register()));
    }
    this->registers.insert(std::pair<unsigned int, Register>(PC, Register()));
    /**
     * We now have registers in the range 0-64.
     * the program counter is at 200
     */
    BOOST_LOG_TRIVIAL(debug) << "done creating registers "<<this;
}


Register *Registers::getRegister(unsigned int x) {
    std::unordered_map<unsigned int, Register>::iterator items = this->registers.find(x);
    if(items == this->registers.end()) {
        BOOST_LOG_TRIVIAL(debug) << "register ID not located input value: "<<x;
        throw "registerIDinvalid";
    }
    return &(items->second);
}
