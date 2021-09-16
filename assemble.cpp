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


char AssembleConstants::registerNameSeperator = ';';
uint AssembleConstants::registerCount = 64;
// BIG list of registers registers with two names are seperated by a ';'
std::string AssembleConstants::registerNames[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0;fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6", "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7", "fs0", "fs1", "fa0", "fa1", "fa2", "fa3", "fa4", "fa5", "fa6", "fa7", "fs2", "fs3", "fs4", "fs5", "fs6", "fs7", "fs8", "Fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11"};

std::list<std::list<std::string>> AssembleConstants::getNamesAsList() {
    std::list<std::list<std::string>> result;
    for(uint i=0; i<AssembleConstants::registerCount; i++) {
        std::string sep;
        sep+=AssembleConstants::registerNameSeperator;
        std::vector<std::string> in;
        in.push_back(sep);
        std::vector<std::string> split = splitStringRemoveEmpty(AssembleConstants::registerNames[i], in);
        std::list<std::string> reg;
        for(std::vector<std::string>::iterator i=split.begin(); i < split.end(); i++) {
            reg.push_back(*i);
        }
        result.push_back(reg);
    }
}

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
    // TODO identify phsudo ops like .dword etc.
    // First step is to split the instruction in to its constituent parts.
    std::vector<std::string> delinators = std::vector<std::string>();
    delinators.push_back(",");
    delinators.push_back(" ");
    Operations op;
    std::vector<SymbolOrRegister> syms;
    std::vector<std::string> split = splitStringRemoveEmpty(this->value, delinators);
    // NOW MAKE SURE THE SYMBOLS.
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
            std::list<std::list<std::string>> names = AssembleConstants::getNamesAsList();
            std::list<std::list<std::string>>::iterator opts = names.begin();
            for(uint reg=0; reg < names.size(); reg++) {
                for(std::list<std::string>::iterator str = opts->begin(); str != opts->end(); str++) {
                    if(*str == *it) {
                        SymbolOrRegister symR;
                        symR.val = *it;
                        symR.t = SymbolOrRegisterType::REGISTER;
                        symR.registerId = reg;
                        syms.push_back(symR);
                    }
                }
                opts++;
            }
            // RISCV is pure load store so only some instructions need this.
            SymbolTableFindResult findResult = this->sym->find(*it);
            if (findResult.found && findResult.symbol != nullptr) {
                SymbolOrRegister symR;
                symR.symbol = findResult.symbol;
                symR.t = SymbolOrRegisterType::SYMBOL;
                symR.val = *it;
                syms.push_back(symR);
                continue;
            } else {
                throw "INVALID SYMBOL";
            }
        }
    }
    Register result = Register();
    if(op == Operations::ADD) {
        // TODO process this put it in the register and then return the register value.
        // syms[0] is dest
        // syms[1] is op
        // syms[2] is op2
        // TODO fix bit pattern
        uint16_t dest = syms.at(0).registerId;
        uint16_t op0 = syms.at(1).registerId;
        uint16_t op1 = syms.at(2).registerId;
        result.writeInstruction((uint16_t) op, dest, op0, op1);
        return result;
    }
    if(op == Operations::ADDI) {
        uint16_t dest = syms.at(0).registerId;
        uint16_t op0 = syms.at(1).registerId;
        uint16_t op1 = syms.at(2).registerId;
        result.writeInstruction((uint16_t)op, dest, op0, op1);
        return result;
    }
    if(op == Operations::SUB) {
        uint16_t dest = syms.at(0).registerId;
        uint16_t op0 = syms.at(1).registerId;
        uint16_t op1 = syms.at(2).registerId;
        result.writeInstruction((uint16_t)op, dest, op0, op1);
        return result;
    }
    if(op == Operations::SUBI) {
        uint16_t dest = syms.at(0).registerId;
        uint16_t op0 = syms.at(1).registerId;
        uint16_t op1 = syms.at(2).registerId;
        result.writeInstruction((uint16_t)op, dest, op0, op1);
        return result;
    }
    // TODO add more commands
    // LOAD OPS will need to be special. 
    // LOAD/store will likely work with 32 bits?
    // LOAD/store OP CODE, REGISTER ID, LOCATION 32 bits

    return result;
}

// FIXME implement step 1
// STEP 1 is identifying symbols and getting addressed for them
// We will do this by figuring out when the `symName:` part is refering to and then recording the address of that command.
// once we have that address it will be usable in load/store ops.
// Blank lines and lines with only `xyz:` should be ingored, and the symbol reference should be set to the current address
// xyz: add a, b, c
// xyz: .dword
// xyz: .sword

/** FIXME ref after. So we need to build the symbol table first.
 * start current address pointer at 0
 * for each line {
 *  blank skip
 *  reference to location set symbol to current address and strip symbol dec
 *  interpret command and store register in to memory.
 *  if stored increment the current address pointer by 64 bits.
 * }
 * 
 */

Program::Program(std::string value) {
    this->value = value;
    this->sym = SymbolTable();
}

void Program::firstStep() {
    uint64_t current_pointer = 0;
    std::vector<std::string> delinators = std::vector<std::string>();
    delinators.push_back("\n");
    delinators.push_back("\r");
    std::vector<std::string> lines = splitStringRemoveEmpty(this->value, delinators);
    for(std::vector<std::string>::iterator line = lines.begin(); line<lines.end(); line++) {
        // IDENFIY IF THERE IS A SYMBOL DEC
        
        // IDENFIFY IF THERE IS A COMMAND
        // IDENFITY IF THERE IS A COMMANd TO CREATE A WORD.
        // create the symbols and assign them addresses
    }
}

void Program::toMemory(Memory* mem) {
    // TODO PASS 1 load locations.
        // PSUDO OPS that write a value
        // ref to point in code.
    // TODO generate an Instruction from each line
    // TODO step through and write for each command.
    mem->write<unsigned long>(0, 0x0);
}


    