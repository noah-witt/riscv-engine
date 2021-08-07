#pragma once

#include "memory.hpp"
#include <unordered_map>

/**
 * @brief a basic register
 * 
 */
class Register {
    protected:
        unsigned long *value;
    public:
        Register();
        ~Register();
        readResult<unsigned char> readByte();
        readResult<unsigned short> readShort();
        readResult<unsigned int> read();
        readResult<unsigned long> readLong();
        bool writeByte(unsigned char);
        bool writeShort(unsigned short);
        bool write(unsigned int);
        bool writeLong(unsigned long);
};

class zeroRegister: public Register {
    public:
        bool writeLong(unsigned long);
};

class Registers {
    // TODO  store all the registers and 
    protected:
        std::unordered_map<unsigned int, Register> registers;
    public:
        Registers();
        ~Registers();
        Register getRegister(unsigned int);
};