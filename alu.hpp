#pragma once
#include "registers.hpp"
#include "memory.h"

class alu {
    public:
        Registers reg;
        Memory mem;
        alu();
        alu(Memory mem);
        void step();
        void loop();
};

