#pragma once
#include "registers.hpp"
#include "memory.h"
#include "string"
#define INSTRUCTION_LENGTH 64

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
    std::string inputRequestMsg;
    inputRequestTypes inputRequestType;
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
};

