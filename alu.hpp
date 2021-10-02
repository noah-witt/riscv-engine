#pragma once
#include "registers.hpp"
#include "memory.h"
#define INSTRUCTION_LENGTH 64

class alu {
    protected:
        Registers reg;
        Memory mem;
    public:
        alu();
        alu(Memory mem);
        Memory *getMem();
        Registers *getReg();
        void step();
        void loop();
};

