#pragma once
#define INSTRUCTION_LENGTH 8
#include "symbol.hpp"
#include "registers.hpp"
#include "memory.h"
#include "string"
#include <iostream>
#include <climits>

struct debugCtlResult {
    int stepCount=0;
    bool exit = false;
};

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
        void setSymTable(SymbolTable* syms);
        SymbolTable * syms;
        void loop(int maxSteps=10000, bool debug = false);
        void loop(std::istream &in, std::ostream &out, int maxSteps=10000, bool debug = false);
};

