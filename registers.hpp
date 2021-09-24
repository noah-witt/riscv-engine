#pragma once

#include "./memory.hpp"
#include <unordered_map>
#include <cstdint>

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
        void writeInstruction(uint16_t a, uint16_t b, uint16_t c, uint16_t d);
        void writeInstruction (uint16_t a, uint16_t b, uint16_t c);
        void writeInstruction(uint16_t a, uint16_t b);
        void writeInstruction(uint16_t a);
        void writeInstructionOffset(uint16_t a, uint16_t offsetFrom, uint32_t offset);
        uint16_t* readInstruction();

        template<typename resultType, int startOffsetBytes = 0>
        resultType customRead();

        template<typename inputType, int startOffsetBytes = 0>
        void customWrite(inputType in);
};

class zeroRegister: public Register {
    public:
        zeroRegister();
        bool writeByte(unsigned char);
        bool writeShort(unsigned short);
        bool write(unsigned int);
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