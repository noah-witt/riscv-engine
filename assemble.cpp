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
    return result;
}

Symbol SymbolTable::remove(std::string symbol){
    std::unordered_map<std::string, Symbol>::iterator item = this->table.find(symbol);
    // TODO check that iterator is not point to end
    // TODO return the element.
}

Instruction::Instruction(std::string value, SymbolTable* sym) {
    this->value = value;
    this->sym = sym;
}

Register Instruction::getInstruction() {
    // First step is to split the instruction in to its constituent parts.
    std::vector<std::string> delinators = std::vector<std::string>();
    delinators.push_back(",");
    delinators.push_back(" ");
    std::vector<std::string> split = splitStringRemoveEmpty(this->value, delinators);
    // NOW MAKE SURE THE SYMBOLS.
    // TODO FIX
    Register result = Register();
    // TODO record the instruction in to this "register"
    return result;
}

Program::Program(std::string value) {
    this->value = value;
    this->sym = SymbolTable();
}

void Program::toMemory(Memory* mem) {
    // TODO generate an Instruction from each line
    // TODO step through and write for each command.
    mem->write<unsigned long>(0, 0x0);
}


    