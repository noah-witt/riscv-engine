/**
 * @file assemble.hpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @version 0.1
 * @date 2021-09-01
 * 
 */

#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <unordered_map>
#include "./registers.hpp"
#include "./memory.h"
#include "./memory.t.hpp"

class AssembleConstants {
    public:
        static char registerNameSeperator;
        static std::string registerNames[];
        static uint registerCount;
        static std::vector<std::vector<std::string>> getNamesAsList();
};

enum class SymbolOrRegisterType {
    UNSET,
    SYMBOL,
    REGISTER,
    LOCATION_OFFSET,
    IMMEDIATE_VALUE,
};

/**
 * @brief represents a symbol
 * 
 */
class Symbol {
    protected:
        std::string symbol;
        unsigned long referenceLocation;
        unsigned int size;
    public:
        Symbol();
        Symbol(std::string symbol, unsigned long address, unsigned int size);
        ~Symbol();
        unsigned long getAddress();
        unsigned int getSize();
        std::string getSymbol();
        bool matchesSymbol(std::string symbol);
        void setRef(unsigned long address);
};

struct SymbolOrRegister {
    SymbolOrRegisterType t;
    std::string val;
    Symbol * symbol = nullptr;
    Register * reg = nullptr; 
    uint registerId = 0; // the zero register
    uint32_t immediate_value = 0;
};

struct SymbolTableFindResult {
    bool found;
    Symbol * symbol = nullptr;
};

enum class Operations: uint16_t{
    // three register operations
    ADD,
    SUB,
    MUL,
    MULH,
    MULHSU,
    MULHU,
    DIV,
    DIVU,
    REM,
    REMU,
    AND,
    OR,
    XOR,
    SLL,
    SLT,
    SLTU,
    SRL,
    SRA,
    ADDW,
    SUBW,
    SLLW,
    SRLW,
    SRAW,
    MULW,
    DIVW,
    DIVUW,
    REMW,
    REMUW,
    //two register, then immediate ops
    ADDI,
    SLTI,
    SLLI,
    SRLI,
    XORI,
    SRAI,
    ADDIW,
    SLLIW,
    SRLIW,
    SRAIW,
    // These are still two reg and immediate but the immediate is an offset from the Program counter in all cases at runtime.
    JALR,
    BEQ,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    // one register, then offset location
    LB,
    LH,
    LW,
    LWU,
    LD,
    LBU,
    LHU,
    SB,
    SH,
    SW,
    SD,
    JAL,

    // one register and immediate
    LUI,
    AUIPC,

    // two register operations

    // Special cases
    // I.E. instructions that are converted to others
    NOP,
    MV,
    NOT,
    NEG,
    NEGW,

    //Extra special
    // things I have just added.
    HALT,
    PRINT,
    INPUTI,
    INPUTB,
    INPUTS,
    INPUTF,
    //TODO add floating point operations.
};

class SymbolTable {
    protected:
        std::unordered_map<std::string, Symbol> table;
    public:
        /**
         * @brief insert a symbol at a fixed location
         * 
         * @param symbol the name
         * @param address the address of the location
         * @param size the size of the thing
         * @return true if it worked
         * @return false if it worked
         */
        bool insert(std::string symbol, unsigned long address, unsigned int size);
        /**
         * @brief Remove a specified symbol.
         * 
         * @param symbol the symbol name to remove
         * @return Symbol the removed symbol.
         */
        Symbol remove(std::string symbol);
        /**
         * @brief Find a symbol
         * 
         * @param symbol the name of the symbol or a NULL
         * @return Symbol the object 
         */
        SymbolTableFindResult find(std::string symbol);
        /**
         * @brief Layout the memory of this symbol table
         * 
         * @param startAddress the address to start the layout from
         * @return true it worked
         * @return false it failed
         */
        bool layout(unsigned long startAddress);
};

struct generatedInstruction {
    std::vector<unsigned long> values;
};

class Instruction {
    protected:
        std::string value;
        SymbolTable* sym;
        ulong address;
    public:
        /**
         * @brief Construct a new Instruction object
         * 
         * @param value the instruction string line
         */
        Instruction(std::string value, SymbolTable* sym, ulong address);
        /**
         * @brief a register with the value
         * @note Free the register after your done with it.
         * @returns generatedInstruction with the value
         * an odd construct returning a register because it is just a place to put bytes.
         */
        generatedInstruction getInstruction();
};

class Program {
    protected:
        std::string value;
        SymbolTable sym;
        void firstStep();
    public:
        Program(std::string value);
        Program(std::ifstream& in);
        /**
         * load a program to memory.
         * @param mem the memory
         */
        void toMemory(Memory* mem);
        // TODO some sort of operation to load a command in to memory.
};
