#include "./alu.hpp"
#include "./registers.hpp"
#include "./assemble.hpp"
#include "memory"
#include "algorithm"

alu::alu() {
    this->reg = Registers();
    this->mem = Memory();
}

Memory *alu::getMem() {
    return &this->mem;
}

Registers *alu::getReg() {
    return &this->reg;
}

void alu::step() {
    unsigned long pc = this->reg.getRegister(PC)->read<unsigned long>();
    Register operation;
    operation.write<unsigned long>(this->mem.read<unsigned long>(pc).payload);
    // TODO read at location and then execute
    Operations op = (Operations)(*((uint16_t *)operation.readInstruction<uint16_t>()[0]));
    // TODO increment pc unless special op.
    if(op>=Operations::ADD && op <=Operations::REMUW) {
        std::array<void *, 4> ops = operation.readInstruction<uint16_t, uint16_t, uint16_t, uint16_t>();
        Register *dest = this->reg.getRegister(*((unsigned int*) ops[1]));
        Register *input1 = this->reg.getRegister(*((unsigned int*) ops[2]));
        Register *input2 = this->reg.getRegister(*((unsigned int*) ops[2]));
        switch(op) {
            case(Operations::ADD):
                dest->write<int>(input1->read<int>()+input2->read<int>());
                break;
            case(Operations::SUB):
                dest->write<int>(input1->read<int>()-input2->read<int>());
                break;
            // TODO more calculations.
            default:
                //TODO some sort of exception
                break;
        }
    }
    pc+=64;
    this->reg.getRegister(PC)->write<unsigned long>(pc);
}
