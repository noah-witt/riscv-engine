/**
 * @file assemble.cpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @brief compile RISC-V assembly
 * @version 0.1
 * @date 2021-08-31
 * 
 * This is a basic two step compiler. First it identifies symbols
 */


#include "./util.hpp"
#include "./assemble.hpp"
#include "./registers.hpp"
#include "./registers.t.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>
#include <boost/algorithm/string.hpp>


char AssembleConstants::registerNameSeperator = ';';
uint AssembleConstants::registerCount = 64;
// BIG list of registers registers with two names are seperated by a ';'
std::string AssembleConstants::registerNames[] = {"zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2", "s0;fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6", "ft0", "ft1", "ft2", "ft3", "ft4", "ft5", "ft6", "ft7", "fs0", "fs1", "fa0", "fa1", "fa2", "fa3", "fa4", "fa5", "fa6", "fa7", "fs2", "fs3", "fs4", "fs5", "fs6", "fs7", "fs8", "fs9", "fs10", "fs11", "ft8", "ft9", "ft10", "ft11"};

std::vector<std::vector<std::string>> AssembleConstants::getNamesAsList() {
    std::vector<std::vector<std::string>> result;
    for(uint i=0; i<AssembleConstants::registerCount; i++) {
        std::string sep;
        sep+=AssembleConstants::registerNameSeperator;
        std::vector<std::string> in;
        in.push_back(sep);
        //BOOST_LOG_TRIVIAL(debug) << "generating list "<<sep<<" "<<i<< " "<<AssembleConstants::registerNames[i];
        std::vector<std::string> split = splitStringRemoveEmpty(AssembleConstants::registerNames[i], in);
        result.push_back(split);
        // FIXME this is what is preventing the issue.
        // works at 39
        // fails at 40
        // it changes sometimes
        //if(i>10) {
        //    return result;
        //}
    }
    return result;
}

Symbol::Symbol(std::string symbol, unsigned long address, unsigned int size) {
    this->symbol = symbol;
    this->size = size;
    this->referenceLocation = address;
}

Symbol::Symbol() {
    this->symbol = "";
    this->size = 0;
    this->referenceLocation = 0;
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

bool SymbolTable::insert(std::string symbol, unsigned long address, unsigned int size) {
    std::unordered_map<std::string, Symbol>::iterator found = this->table.find(symbol);
    if(found != this->table.end()) {
        return false;
    }
    this->table.insert(std::pair<std::string, Symbol>(symbol, Symbol(symbol, address, size)));
    return true;
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
    return item->second;
}

Instruction::Instruction(std::string value, SymbolTable* sym, ulong a) {
    this->value = value;
    this->sym = sym;
    this->address = a;
    // TODO DETECT AND LOAD SYMBOLS.
}

long decodeValue(const std::string &in) {
    BOOST_LOG_TRIVIAL(debug) <<"decoding: "<< in;
    //first check if it starts with 0x
    if(in.size()>2 && in.at(0)=='0' && in.at(1)=='x') {
        BOOST_LOG_TRIVIAL(debug) << "decoding as hex";
        in.substr(2);
        char * p;
        return std::strtol(in.substr(2).c_str(), &p, 16);
    }
    // ascii character
    if(in.size()==3&&in.at(0)=='\''&&in.at(2)=='\'') {
        BOOST_LOG_TRIVIAL(debug) <<"decoding as char";
        return (long)in.at(1);
    }
    //decimal int
    BOOST_LOG_TRIVIAL(debug) <<"decoding using stol";
    long result =  std::stol(in);
    return result;
}

generatedInstruction Instruction::getInstruction() {
    BOOST_LOG_TRIVIAL(debug) << "at get instruction";
    // TODO identify labels
    // TODO identify pseudo ops like .dword etc.
    // First step is to split the instruction in to its constituent parts.
    std::vector<std::string> delinators = std::vector<std::string>();
    delinators.push_back(",");
    delinators.push_back(" ");
    Operations op;
    std::vector<SymbolOrRegister> syms;
    BOOST_LOG_TRIVIAL(debug) << "prcessing line " << this->value;
    std::vector<std::string> split = splitStringRemoveEmpty(this->value, delinators);
    // NOW MAKE SURE THE SYMBOLS.
    std::vector<std::string>::iterator it = split.begin();
    //if the line is a comment just continue
    if((*it).at(0)=='#'||(*it).size()==0) {
        // empty generated instruction because this is a comment
        return generatedInstruction();
    }
    for(;it<split.end();it++) {
        if (it->at(0) == '.') {
            // FIXME handle things that start with .
            if(it != split.begin()) {
                // dot not at the first line.
                throw "key that begins with . other than at first of a line for pseudo op.";
            }
            // we know this is the first line now we can do our logic.
            std::string opType = std::string(*it);
            trim(opType);
            boost::to_upper(opType);
            generatedInstruction result;
            Register reg;
            BOOST_LOG_TRIVIAL(debug) << "identifying the mem op type. :"<<opType;
            if(opType==".ASCIIZ") {
                std::vector<std::string> quote;
                quote.push_back("\"");
                BOOST_LOG_TRIVIAL(debug) << "asciiz line";
                std::vector<std::string> quotedString = splitStringRemoveEmpty(this->value, quote);
                for(uint at=0; at<quotedString.at(1).length();at++) {
                    reg.write<char>(quotedString.at(1).at(at));
                    result.values.push_back(reg.read<ulong>());
                }
                reg.write<char>('\0');
                result.values.push_back(reg.read<ulong>());
                return result;
                // process an asci string.
            }
            long val = decodeValue(*(it+1));
            if(opType==".BYTE") {
                // do things with a byte
                reg.write<char>((char)val);
                result.values.push_back(reg.read<ulong>());
                return result;
            }
            if(opType==".HALF") {
                reg.write<short>((short)val);
                result.values.push_back(reg.read<ulong>());
                return result;
            }
            if(opType==".WORD") {
                reg.write<int>((int)val);
                result.values.push_back(reg.read<ulong>());
                return result;
            }
            if(opType==".DWORD") {
                reg.write<long>((long)val);
                result.values.push_back(reg.read<ulong>());
                return result;
            }
            throw "unknown pseudo operation.";
            //TODO this only breaks from the part of the line so we need to set some flag to stop the loop from continuing.
            break; // the whole line is handled here if it starts with ..
        }
        if (it == split.begin()) {
            // trim and uppercase the string.
            std::string cmd = std::string(*it);
            trim(cmd);
            boost::to_upper(cmd);
            // the command

            //special cases
            if(cmd == "NOP") {
                op = Operations::NOP;
            }
            if(cmd=="MV") {
                op = Operations::MV;
            }
            if(cmd=="NOT") {
                op = Operations::NOT;
            }
            if(cmd=="NEG") {
                op = Operations::NEG;
            }
            if(cmd=="NEGW") {
                op = Operations::NEGW;
            }
            if(cmd == "HALT") {
                op = Operations::HALT;
            }
            if(cmd=="PRINT") {
                op = Operations::PRINT;
            }
            if(cmd=="INPUTI"){
                op = Operations::INPUTI;
            }
            if(cmd=="INPUTB"){
                op = Operations::INPUTB;
            }
            if(cmd=="INPUTS"){
                op = Operations::INPUTS;
            }
            if(cmd=="INPUTF"){
                op = Operations::INPUTF;
            }
            if(cmd=="DEBUG") {
                op = Operations::DEBUG;
            }


            // three register operations
            if(cmd == "ADD") {
                op = Operations::ADD;
            }
            if(cmd == "SUB") {
                op = Operations::SUB;
            }
            if(cmd == "MUL") {
                op = Operations::MUL;
            }
            if(cmd == "MULH") {
                op = Operations::MULH;
            }
            if(cmd == "MULHSU") {
                op = Operations::MULHSU;
            }
            if(cmd == "MULHU") {
                op = Operations::MULHU;
            }
            if(cmd == "DIV") {
                op = Operations::DIV;
            }
            if(cmd == "DIVU") {
                op = Operations::DIVU;
            }
            if(cmd == "REM") {
                op = Operations::REM;
            }
            if(cmd == "REMU") {
                op = Operations::REMU;
            }
            if(cmd == "AND") {
                op = Operations::AND;
            }
            if(cmd == "OR") {
                op = Operations::OR;
            }
            if(cmd == "XOR") {
                op = Operations::XOR;
            }
            if(cmd == "SLL") {
                op = Operations::SLL;
            }
            if(cmd == "SLT") {
                op = Operations::SLT;
            }
            if(cmd == "SLTU") {
                op = Operations::SLTU;
            }
            if(cmd == "SRL") {
                op = Operations::SRL;
            }
            if(cmd == "SRA") {
                op = Operations::SRA;
            }
            if(cmd == "ADDW") {
                op = Operations::ADDW;
            }
            if(cmd == "SUBW") {
                op = Operations::SUBW;
            }
            if(cmd == "SLLW") {
                op = Operations::SLLW;
            }
            if(cmd == "SRLW") {
                op = Operations::SRLW;
            }
            if(cmd == "SRAW") {
                op = Operations::SRAW;
            }
            if(cmd == "MULW") {
                op = Operations::MULW;
            }
            if(cmd == "DIVW") {
                op = Operations::DIVW;
            }
            if(cmd == "DIVUW") {
                op = Operations::DIVUW;
            }
            if(cmd == "REMW") {
                op = Operations::REMW;
            }
            if(cmd == "REMUW") {
                op = Operations::REMUW;
            }

            // two register, then immediate ops section
            if(cmd == "ADDI") {
                op = Operations::ADDI;
            }
            if(cmd == "SLTI") {
                op = Operations::SLTI;
            }
            if(cmd == "SLLI") {
                op = Operations::SLLI;
            }
            if(cmd == "SRLI") {
                op = Operations::SRLI;
            }
            if(cmd == "SRAI") {
                op = Operations::SRAI;
            }
            if(cmd == "ADDIW") {
                op = Operations::ADDIW;
            }
            if(cmd == "SLLIW") {
                op = Operations::SLLIW;
            }
            if(cmd == "SRAI") {
                op = Operations::SRAI;
            }
            if(cmd == "ADDIW") {
                op = Operations::ADDIW;
            }
            if(cmd == "SLLIW") {
                op = Operations::SRAI;
            }
            if(cmd == "SRLIW") {
                op = Operations::SRLIW;
            }
            if(cmd == "SRAIW") {
                op = Operations::SRAIW;
            }
            //two reg and immediate still but offset from pc.
            if(cmd == "JALR") {
                op = Operations::JALR;
            }
            if(cmd == "BEQ") {
                op = Operations::BEQ;
            }
            if(cmd == "BNE") {
                op = Operations::BNE;
            }
            if(cmd == "BLT") {
                op = Operations::BLT;
            }
            if(cmd == "BGE") {
                op = Operations::BGE;
            }
            if(cmd == "BLTU") {
                op = Operations::BLTU;
            }
            if(cmd == "BGEU") {
                op = Operations::BGEU;
            }

            // one register then offset location
            if(cmd == "LB") {
                op = Operations::LB;
            }
            if(cmd == "LH") {
                op = Operations::LH;
            }
            if(cmd == "LW") {
                op = Operations::LW;
            }
            if(cmd == "LWU") {
                op = Operations::LWU;
            }
            if(cmd == "LD") {
                op = Operations::LD;
            }
            if(cmd == "LBU") {
                op = Operations::LBU;
            }
            if(cmd == "LHU") {
                op = Operations::LHU;
            }
            if(cmd == "SB") {
                op = Operations::SB;
            }
            if(cmd == "SH") {
                op = Operations::SW;
            }
            if(cmd == "SD") {
                op = Operations::SD;
            }
            if(cmd == "JAL") {
                op = Operations::JAL;
            }

            // one register than immediate
            if(cmd == "LUI") {
                op = Operations::LUI;
            }
            if(cmd == "AUIPC") {
                op = Operations::AUIPC;
            }

            // FIXME add more.
        } else if(it->find('(') != std::string::npos && it->find(')') != std::string::npos) {
            // FIXME untested
            //identify memory refs 123(sp) or -64(sp) or other register name
            std::vector<std::string> openAndClose;
            openAndClose.push_back("(");
            openAndClose.push_back(")");
            std::vector<std::string> itemParts = splitStringRemoveEmpty(*it, openAndClose);
            if(itemParts.size()!=2) {
                BOOST_LOG_TRIVIAL(debug) << "not continuing due to fail to process " << *it;
                continue;
            }
            // get the offset part
            uint32_t offset = std::stoi(itemParts[0]);
            //FIXME allow named things
            // get the register part.
            int regOffsetFrom = -1;
            std::vector<std::vector<std::string>> names = AssembleConstants::getNamesAsList();
            std::vector<std::vector<std::string>>::iterator opts = names.begin();
            bool regNameFound = false;
            for(uint reg=0; reg < names.size(); reg++) {
                if(opts==names.end()) {
                    throw "OPTS BEYOND END";
                }
                for(std::vector<std::string>::iterator str = opts->begin(); !regNameFound && str != opts->end(); str++) {
                    if(*str == itemParts[1]) {
                        regOffsetFrom = reg;
                        regNameFound = true;
                    }
                }
                opts++;
            }
            if(regOffsetFrom<0) {
                BOOST_LOG_TRIVIAL(debug) << "register name not found "<<itemParts[1];
                continue;
            }
            SymbolOrRegister symR;
            symR.t = SymbolOrRegisterType::LOCATION_OFFSET;
            symR.registerId = regOffsetFrom;
            symR.immediate_value = offset;
            syms.push_back(symR);
        } else if(isNumber(*it)) {
            // FIXME immediate value
            // FIXME UNTESTED
            uint32_t immediate = std::stoi(*it);
            SymbolOrRegister symR;
            symR.t = SymbolOrRegisterType::IMMEDIATE_VALUE;
            symR.immediate_value = immediate;
            syms.push_back(symR);
        } else {
            std::vector<std::vector<std::string>> names = AssembleConstants::getNamesAsList();
            std::vector<std::vector<std::string>>::iterator opts = names.begin();
            bool regNameFound = false;
            for(uint reg=0; reg < names.size(); reg++) {
                for(std::vector<std::string>::iterator str = opts->begin(); !regNameFound && str != opts->end(); str++) {
                    if(*str == *it) {
                        BOOST_LOG_TRIVIAL(debug) << "identified as register "<<reg;
                        SymbolOrRegister symR;
                        symR.val = *it;
                        symR.t = SymbolOrRegisterType::REGISTER;
                        symR.registerId = reg;
                        syms.push_back(symR);
                        regNameFound = true;
                    }
                }
                opts++;
            }
            // RISC-V is pure load store so only some instructions need this.
            SymbolTableFindResult findResult = this->sym->find(*it);
            if (regNameFound) {
                continue;
            } else if (findResult.found && findResult.symbol != nullptr) {
                SymbolOrRegister symR;
                symR.symbol = findResult.symbol;
                symR.t = SymbolOrRegisterType::SYMBOL;
                symR.val = *it;
                symR.immediate_value = (uint32_t)findResult.symbol->getAddress();
                syms.push_back(symR);
                continue;
            } else {
                throw "INVALID SYMBOL";
            }
        }
    }
    BOOST_LOG_TRIVIAL(debug) << "get instruction process each part sym count "<<syms.size() << " op: "<<(uint16_t)op;
    Register result = Register();
    // 3 register param options.
    if(op>=Operations::ADD && op <=Operations::REMUW) {
        // these operations get stored like this (uint16) op, (uint16) op, (uint16) reg1, (uint16) reg2, (uint16) reg3.
        uint16_t dest = syms.at(0).registerId;
        uint16_t op0 = syms.at(1).registerId;
        uint16_t op1 = syms.at(2).registerId;
        BOOST_LOG_TRIVIAL(debug) << "writing three register op " << (uint16_t)op<<" "<< dest << " " << op0 << " " << op1;
        result.writeInstruction((uint16_t) op, dest, op0, op1);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }
    // 2 reg and then immediate
    if(op>=Operations::ADDI && op <= Operations::BGEU) {
        // TODO change to 32 bit im value.
        uint8_t dest = syms.at(0).registerId;
        uint8_t op0 = syms.at(1).registerId;
        uint32_t im = syms.at(2).immediate_value;
        // TODO FIX ME too many bits
        result.writeInstruction<uint16_t, uint8_t, uint8_t, int32_t>((uint16_t) op, dest, op0, im);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }
    /**
     * memory operations
     * memory operations have three parts. a register, an access mode and an address. 
     * for memory ops we split it in to three parts. (uint16) op, (uint8) register, (uint8) register to offset from, (uint32) address.
     * the mode dictates if we reference off of stackpointer for example.
     */
    // 1 reg then offset location
    if(op>=Operations::LB && op<=Operations::JAL) {
        uint8_t dest = syms.at(0).registerId;
        uint8_t offsetFrom = syms.at(1).registerId; // defaults to the zero register
        uint32_t offset = syms.at(1).immediate_value;
        result.writeInstruction<uint16_t, uint8_t, uint8_t, int32_t>((uint16_t) op, dest, offsetFrom, offset);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }

    // TODO one register and immediate
    if(op>=Operations::LUI && op<=Operations::AUIPC) {
        //load an immediate value in a register
        uint8_t dest = syms.at(0).registerId;
        int32_t val = syms.at(1).immediate_value;
        result.writeInstruction<uint16_t, uint8_t, int32_t, uint8_t>((uint16_t) op, dest, val, 0);
        generatedInstruction gen;
        gen.values.push_back(result.read<unsigned long>());
        return gen;
    }

    // special operations
    if(op==Operations::NOP) {
        // NO OP is translated to the add x0, x0, x0 instruction.
        result.writeInstruction<uint16_t, uint16_t, uint16_t, uint16_t>((uint16_t) Operations::ADD, 0, 0, 0);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }

    if(op==Operations::MV) {
        uint16_t a = syms.at(0).registerId;
        uint16_t b = syms.at(1).registerId;
        result.writeInstruction<uint16_t, uint8_t, uint8_t, uint32_t>((uint16_t) Operations::ADDI, a, b, 0);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }
    // TODO fix more special.
    if(op==Operations::NOT) {
        uint16_t a = syms.at(0).registerId;
        uint16_t b = syms.at(1).registerId;
        result.writeInstruction<uint16_t, uint8_t, uint8_t, int32_t>((uint16_t) Operations::XORI, a, b, -1);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }

    if(op==Operations::NEG) {
        uint16_t a = syms.at(0).registerId;
        uint16_t b = syms.at(1).registerId;
        result.writeInstruction<uint16_t, uint16_t, uint16_t, uint16_t>((uint16_t) Operations::SUB, a, 0, b);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }

    if(op==Operations::NEGW) {
        uint16_t a = syms.at(0).registerId;
        uint16_t b = syms.at(1).registerId;
        result.writeInstruction<uint16_t, uint16_t, uint16_t, uint16_t>((uint16_t) Operations::SUBW, a, 0, b);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }

    if(op==Operations::HALT) {
        result.writeInstruction<uint16_t, uint16_t, uint16_t, uint16_t>((uint16_t) Operations::HALT, 0, 0, 0);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }

    if(op==Operations::PRINT) {
        // a print command has three paramaters. A print type.
        // it is ordered like: print, type (8), offsetFrom(8), offset(32)
        // the offset points to an ascii string in memory.
        uint8_t type = syms.at(0).immediate_value;
        uint8_t offsetFrom = syms.at(1).registerId; // defaults to the zero register
        uint32_t offset = syms.at(1).immediate_value;
        result.writeInstruction<uint16_t, uint8_t, uint8_t, int32_t>((uint16_t) op, type, offsetFrom, offset);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }

    if(op>=Operations::INPUTI && op<=Operations::INPUTF) {
        // an input instruction contains the memory location that the input should be written in as an immediate value offset from some reg
        // takes one param like "INPUTF 32412" where 3412 is the memory location that the data is going to be written.
        // it can also be a register id. the immediate value is read as an offset from the register provided.
        result.writeInstruction<uint16_t, uint8_t, int32_t, uint8_t>((uint16_t) op,  syms.at(0).registerId,  syms.at(0).immediate_value, 0);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }
    if(op==Operations::DEBUG) {
        result.writeInstruction<uint16_t, uint16_t, int8_t, uint8_t>((uint16_t) op,  syms.at(0).registerId,0, 0);
        generatedInstruction val;
        val.values.push_back(result.read<unsigned long>());
        return val;
    }
    
    throw "OP NOT IMPLEMENTED";
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

Program::Program(std::ifstream& in) {
    std::string content( (std::istreambuf_iterator<char>(in) ),
                       (std::istreambuf_iterator<char>()    ) );
    this->value = content;
    this->sym = SymbolTable();
}

void Program::firstStep() {
    // TODO free the old symbol table and setup a new one
    uint64_t current_pointer = 0;
    std::vector<std::string> delinators = std::vector<std::string>();
    delinators.push_back("\n");
    delinators.push_back("\r");
    std::vector<std::string> lines = splitStringRemoveEmpty(this->value, delinators);

    // SOME ISSUE HERE processing each line.
    for(std::vector<std::string>::iterator line = lines.begin(); line!=lines.end(); line++) {
        // IDENTIFY IF THERE IS A SYMBOL DEC
        std::vector<std::string> lineDelinator = std::vector<std::string>();
        lineDelinator.push_back(" ");
        std::vector<std::string> parts = splitStringRemoveEmpty(*line, lineDelinator);
        //if the line is a comment just continue
        if(parts.size()>0 && parts[0].at(0)=='#') continue;
        for(std::vector<std::string>::iterator part = parts.begin(); part<parts.end(); part++) {
            if(strEndsIn(*part, ":")) {
                std::string name = part->substr(0, part->size()-1);
                this->sym.insert(name, current_pointer, 64);
                continue;
            }
            trim(*part);
            boost::to_upper(*part);
            if(! part->empty()) {
                if(*part==".BYTE") {
                    current_pointer+=64;
                    break;
                }
                if(*part==".HALF") {
                    current_pointer+=64;
                    break;
                }
                if(*part==".WORD") {
                    current_pointer+=64;
                    break;
                }
                if(*part==".DWORD") {
                    current_pointer+=64;
                    break;
                }
                if(*part==".ASCIIZ") {
                    //this command calculates the number of characters between the quote marks and then adds one for the null terminate.
                    std::vector<std::string> quote;
                    quote.push_back("\"");
                    std::vector<std::string> quoteParts = splitStringRemoveEmpty(*line,quote);
                    int bytes = quoteParts.at(1).size()+1;
                    current_pointer+=bytes*64;
                    break;
                }
                current_pointer+=64;
                break;
            }
        }
    }
}

void Program::toMemory(Memory* memoryInput) {
    BOOST_LOG_TRIVIAL(debug) << "at start of to memory";
    this->firstStep();
    uint64_t current_pointer = 0;
    std::vector<std::string> delinators = std::vector<std::string>();
    delinators.push_back("\n");
    delinators.push_back("\r");
    std::vector<std::string> lines = splitStringRemoveEmpty(this->value, delinators);
    for(std::vector<std::string>::iterator line = lines.begin(); line!=lines.end(); line++) {
        BOOST_LOG_TRIVIAL(debug) <<"starting to process line "<<*line;
        // this is each line.
        // TODO Process each line dropping the :
        std::vector<std::string> partDelinators = std::vector<std::string>();
        partDelinators.push_back(":");
        std::vector<std::string> parts = splitStringRemoveEmpty(*line, partDelinators);
        std::string operation;
        if(parts.size()>2) {
            // There should not be more than one of these in a line.
            throw std::exception();
        }
        if(parts.size()==2) {
            // discard the first part.
            operation = parts[1];
            // FIXME make sure this matches the map...
            // by comparing the current key and the current pointer address.
        }
        if(parts.size()==1) {
            // if it contains the : we should set it to empty else we need to set it to parts[0]
            if(line->find(':')==std::string::npos) {
                operation = parts[0];
            } else {
                operation = "";
            }
        }
        //remove leading and trailing whitespace.
        trim(operation);
        if(operation.empty()){
            // just continue to the next line.
            continue;
        }
        // now we have a command to pass to the instruction register generator.
        Instruction instruction = Instruction(operation, &this->sym, (ulong)current_pointer);
        try {
            BOOST_LOG_TRIVIAL(debug) << "generating instruction";
            generatedInstruction gen = instruction.getInstruction();
            for(std::vector<unsigned long>::iterator val = gen.values.begin(); val<gen.values.end(); val++) {
                BOOST_LOG_TRIVIAL(debug) << "writing instruction to memory at "<<current_pointer<<"; with value: "<< *val <<" in " << memoryInput;
                memoryInput->write<unsigned long>(current_pointer, *val);
                BOOST_LOG_TRIVIAL(debug) << "advancing current pointer and continuing";
                current_pointer+=64; // FIXME handle lengths that are different than 64 bits.
            }
            BOOST_LOG_TRIVIAL(debug) <<"done writing parts";
        } catch (std::exception e) {
            BOOST_LOG_TRIVIAL(debug) << "error in to memory "<< e.what();
            // TODO some sort of logging to record this.
            // It probably is nothing though because it could just be a non command.
            throw e; //FIXME remove this line
        }
        BOOST_LOG_TRIVIAL(debug) <<"going to the next line";
    }
    // the memory image should be written.
}


    