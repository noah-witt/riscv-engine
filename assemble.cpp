/**
 * @file assemble.cpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @brief compile RISC-V assembly
 * @version 0.1
 * @date 2021-08-31
 * 
 * This is a basic two step compiler. First it identifies symbols
 */

#include <util.hpp>
#include <assemble.hpp>
#include <string>
#include <vector>



Symbol::Symbol(std::string symbol, unsigned long address, unsigned int size) {
    this->symbol = symbol;
    this->size = size;
    this->referenceLocation = address;
}

Symbol::Symbol() {
    this->symbol = "";
    this->size = 0;
    this->referenceLocation = NULL;
}

Symbol::~Symbol() {
    // TODO implement
}


unsigned int Symbol::getSize() {
    return this->size;
}

std::string Symbol::getSymbol() {
    return this->symbol;
}

bool Symbol::matchesSymbol(std:: string s) {
    return this->symbol == s;
}

unsigned long Symbol::getAddress() {
    return this->referenceLocation;
}

void Symbol::setRef(unsigned long address) {
    this->referenceLocation = address;
}

SymbolTableFindResult SymbolTable::find(std::string symbol) {
    SymbolTableFindResult result;
    result.found = false;
    std::unordered_map<std::string, Symbol>::iterator it = this->table.find(symbol);
    if (it != this->table.end()) {
        result.found = true;
        result.symbol = &((*it).second);
    }
    return result;
}

Symbol SymbolTable::remove(std::string symbol){
    std::unordered_map<std::string, Symbol>::iterator item = this->table.find(symbol);
    // TODO check that iterator is not point to end
    // TODO return the element.
}

Instruction::Instruction(std::string value, SymbolTable* sym, ulong a) {
    this->value = value;
    this->sym = sym;
    this->address = a;
    // TODO DETECT AND LOAD SYMBOLS.
}

Register Instruction::getInstruction() {
    // TODO identify labels
    // First step is to split the instruction in to its constituent parts.
    std::vector<std::string> delinators = std::vector<std::string>();
    delinators.push_back(",");
    delinators.push_back(" ");
    Operations op;
    std::vector<Symbol *> syms;
    std::vector<std::string> split = splitStringRemoveEmpty(this->value, delinators);
    // NOW MAKE SURE THE SYMBOLS.
    // TODO FIX
    std::vector<std::string>::iterator it = split.begin();
    for(;it<split.end();it++) {
        if (it == split.begin()) {
            // the command
            if(*it == "ADD") {
                op = Operations::ADD;
            }
            if(*it == "ADDI") {
                op = Operations::ADDI;
            }
            if(*it == "SUB") {
                op = Operations::SUB;
            }
            if(*it == "SUBI") {
                op = Operations::SUBI;
            }
            // FIXME add more.
        } else {
            // TODO IDENTIFY IF REGISTER OR MEMORY SYMBOL NAME.
            SymbolTableFindResult findResult = this->sym->find(*it);
            if (findResult.found && findResult.symbol != nullptr) {
                syms.push_back(findResult.symbol);
            } else {
                throw "INVALID SYMBOL";
            }
        }
    }
    Register result = Register();
    // TODO record the instruction in to this "register"
    return result;
}

Program::Program(std::string value) {
    this->value = value;
    this->sym = SymbolTable();
}

void Program::toMemory(Memory* mem) {
    // TODO PASS 1 load locations.
    // TODO generate an Instruction from each line
    // TODO step through and write for each command.
    mem->write<unsigned long>(0, 0x0);
}


    