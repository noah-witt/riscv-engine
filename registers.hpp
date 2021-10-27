#pragma once

#include "./memory.hpp"
#include <unordered_map>
#include <array>
#include <cstdint>
# define REGISTER_WIDTH_BYTES 8
# define REGISTERS_END 64
# define PC 200

/**
 * @brief a basic register
 * 
 */
class Register {
    protected:
        bool isMutable;
        unsigned long *value;
    public:
        Register();
        
        template<typename a = uint16_t, typename b = void, typename c = void, typename d = void>
        void writeInstruction(const a& va, const b& vb, const c& vc, const d& vd);


        template<typename a = uint16_t, typename b = void, typename c = void, typename d = void>
        std::array<void*,4> readInstruction();

        template<typename a = uint16_t, typename b = void, typename c = void, typename d = void>
        std::array<long, 4> readInstructionNormalized();

        template<typename resultType, int startOffsetBytes = 0, bool neg = false>
        resultType read();

        template<typename inputType, int startOffsetBytes = 0, bool neg = false>
        void write(const inputType& in);

        template<typename inputType>
        void writeLower(const inputType& in);

        template<typename resultType>
        resultType readLower();
};

class zeroRegister: public Register {
    public:
        zeroRegister();

};

class Registers {
    // TODO  store all the registers and 
    protected:
        std::unordered_map<unsigned int, Register> registers;
    public:
        Registers();
        Register *getRegister(unsigned int);
};