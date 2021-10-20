#pragma once
#include "registers.hpp"
#include "memory.h"
#include "string"
#include <iostream>
#include <climits>
#define INSTRUCTION_LENGTH 8

enum class inputRequestTypes {
    BOOL,
    INT,
    FLOAT,
    STR,
};
struct AluStepResult {
    bool halt = false;
    bool printStr = false;
    std::string printStrValue;
    bool inputRequest = false;
    inputRequestTypes inputRequestType;
    unsigned long inputLocation = ULONG_MAX;
};

class alu {
    protected:
        Registers reg;
        Memory mem;
    public:
        Memory *getMem();
        Registers *getReg();
        AluStepResult step();
        void loop(int maxSteps=10000);
        void loop(std::istream &in, std::ostream &out, int maxSteps=10000);
};

