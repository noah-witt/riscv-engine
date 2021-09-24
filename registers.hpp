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
        ~Register();
        
        template<typename a = uint16_t, typename b = void, typename c = void, typename d = void>
        void writeInstruction(const a& va, const b& vb, const c& vc, const d& vd);


        template<typename a = uint16_t, typename b = void, typename c = void, typename d = void>
        std::array<void*,4> readInstruction();

        template<typename resultType, int startOffsetBytes = 0>
        resultType* read();

        template<typename inputType, int startOffsetBytes = 0>
        void write(const inputType& in);

        template<typename inputType>
        void writeLower(const inputType& in);

        template<typename resultType>
        resultType* readLower();
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
        ~Registers();
        Register getRegister(unsigned int);
};