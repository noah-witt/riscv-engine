/**
 * @file registers.cpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @brief Provides registers
 * @version 0.1
 * @date 2021-08-05
 * 
 * 
 */

#include "registers.hpp"
#include "memory.hpp"
#include <stdlib.h>
#include <unordered_map>
# define REGISTER_WIDTH_BYTES 8
# define REGISTERS_END 31
# define F_REGISTERS_START 100
# define F_REGISTERS_COUNT 32
# define PC 200

Register::Register() {
    this->value = (unsigned long *)calloc(REGISTER_WIDTH_BYTES, sizeof(unsigned char));
}

Register::~Register() {
    free(this->value);
}

readResult<unsigned char> Register::readByte() {
    readResult<unsigned char> result;
    try {
        result.payload = (unsigned char)*(this->value);
        result.valid = true;
        return result;
    } catch(int e) {
        result.valid = false;
        result.payload = 0x0;
    }
    return result;
}

readResult<unsigned short> Register::readShort() {
    readResult<unsigned short> result;
    try {
        result.payload = (unsigned short)*(this->value);
        result.valid = true;
        return result;
    } catch(int e) {
        result.valid = false;
        result.payload = 0x0;
    }
    return result;
}

readResult<unsigned int> Register::read() {
    readResult<unsigned int> result;
    try {
        result.payload = (unsigned int)*(this->value);
        result.valid = true;
        return result;
    } catch(int e) {
        result.valid = false;
        result.payload = 0x0;
    }
    return result;
}

readResult<unsigned long> Register::readLong() {
    readResult<unsigned long> result;
    try {
        result.payload = (unsigned long)*(this->value);
        result.valid = true;
        return result;
    } catch(int e) {
        result.valid = false;
        result.payload = 0x0;
    }
    return result;
}

bool Register::writeByte(unsigned char in) {
    return this->writeLong((unsigned long) in);
}

bool Register::writeShort(unsigned short in) {
    return this->writeLong((unsigned long) in);
}

bool Register::write(unsigned int in) {
    return this->writeLong((unsigned int) in);
}

bool Register::writeLong(unsigned long in) {
    *this->value = in;
    return true;
}


zeroRegister::zeroRegister() {
    this->value = (unsigned long *)calloc(REGISTER_WIDTH_BYTES, sizeof(unsigned char));
    *this->value =   0x0;
}

bool zeroRegister::writeByte(unsigned char in) {
    return false;
}

bool zeroRegister::writeShort(unsigned short in) {
    return false;
}

bool zeroRegister::write(unsigned int in) {
    return false;
}

bool zeroRegister::writeLong(unsigned long in) {
    return false;
}


Registers::Registers() {
    this->registers = std::unordered_map<unsigned int, Register>();
    this->registers.insert(std::pair<unsigned int, Register>(0, zeroRegister()));
    for(unsigned int i=1; i<=REGISTERS_END; i++) {
        //create registers for [1, REGISTERS_END]
        this->registers.insert(std::pair<unsigned int, Register>(i, Register()));
    }
    this->registers.insert(std::pair<unsigned int, Register>(PC, Register()));
    for(unsigned int i=F_REGISTERS_START; i<F_REGISTERS_START+F_REGISTERS_COUNT; i++) {
        this->registers.insert(std::pair<unsigned int, Register>(i, Register()));
    }
    /**
     * We now have registers in the range 0-31.
     * the program counter is at 200
     * and fp registers in the range 100-131
     */
}

Registers::~Registers() {
    std::unordered_map<unsigned int, Register>::iterator it = this->registers.begin();
    while(it != this->registers.end()) {
        it->second.~Register();
    }
    this->registers.~unordered_map();
}

Register Registers::getRegister(unsigned int x) {
    std::unordered_map<unsigned int, Register>::iterator items = this->registers.find(x);
    return items->second;
}