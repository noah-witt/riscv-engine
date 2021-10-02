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
    BOOST_LOG_TRIVIAL(debug) << "stepping with memory " << &this->mem << ", and pc " << pc;
    Register operation;
    operation.write<unsigned long>(this->mem.read<unsigned long>(pc).payload);
    BOOST_LOG_TRIVIAL(debug) << "reg value " << operation.read<unsigned long>();
    // TODO read at location and then executessssss
    Operations op = (Operations)(*((uint16_t *)operation.readInstruction<uint16_t>()[0]));
    // TODO increment pc unless special op.
    if(op>=Operations::ADD && op <=Operations::REMUW) {
        std::array<void *, 4> ops = operation.readInstruction<uint16_t, uint16_t, uint16_t, uint16_t>();
        Register *dest = this->reg.getRegister(*((uint16_t*) ops[1]));
        Register *input1 = this->reg.getRegister(*((uint16_t*) ops[2]));
        Register *input2 = this->reg.getRegister(*((uint16_t*) ops[3]));
        switch(op) {
            case(Operations::ADD):
                dest->write<int>(input1->read<int>()+input2->read<int>());
                break;
            case(Operations::SUB):
                dest->write<int>(input1->read<int>()-input2->read<int>());
                break;
            case(Operations::MUL):
                dest->write<int>(input1->read<int>()*input2->read<int>());
                break;
            case(Operations::MULH):
                dest->write<int16_t>(input1->read<int16_t>()*input2->read<int16_t>());
                break;
            case(Operations::MULHSU):
                dest->write<int16_t>(input1->read<int16_t>()*input2->read<uint16_t>());
                break;
            case(Operations::MULHU):
                dest->write<uint16_t>(input1->read<uint16_t>()*input2->read<uint16_t>());
                break;
            case(Operations::DIV):
                dest->write<int>(input1->read<int>()/input2->read<int>());
                break;
            case(Operations::DIVU):
                dest->write<uint>(input1->read<uint>()*input2->read<uint>());
                break;
            case(Operations::REM):
                dest->write<int>(input1->read<int>()%input2->read<int>());
                break;
            case(Operations::REMU):
                dest->write<uint>(input1->read<uint>()%input2->read<uint>());
                break;
            case(Operations::AND):
                dest->write<uint>(input1->read<uint>() & input2->read<uint>());
                break;
            case(Operations::OR):
                dest->write<uint>(input1->read<uint>() | input2->read<uint>());
                break;
            case(Operations::XOR):
                dest->write<uint>(input1->read<uint>() ^ input2->read<uint>());
                break;
            case(Operations::SLL):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SLT):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SLTU):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SRL):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SRA):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            // 64 bit operations
            case(Operations::ADDW):
                dest->write<long>(input1->read<long>()+input2->read<long>());
            case(Operations::SUBW):
                dest->write<long>(input1->read<long>()-input2->read<long>());
            case(Operations::SLLW):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SRLW):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SRAW):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::MULW):
                dest->write<long>(input1->read<long>()*input2->read<long>());
            case(Operations::DIVW):
                dest->write<long>(input1->read<long>()/input2->read<long>());
            case(Operations::DIVUW):
                dest->write<ulong>(input1->read<ulong>()/input2->read<ulong>());
            case(Operations::REMW):
                dest->write<long>(input1->read<long>()%input2->read<long>());
                break;
            case(Operations::REMUW):
                dest->write<ulong>(input1->read<ulong>()%input2->read<ulong>());
                break;
            // TODO more calculations.
            default:
                //TODO some sort of exception
                throw "NOT IMPLEMENTED";
                break;
        }
    } else if(op>=Operations::ADDI && op <=Operations::BGEU) {
        std::array<void *, 4> ops = operation.readInstruction<uint16_t, uint8_t, uint8_t, int32_t>();
        Register *dest = this->reg.getRegister(*((uint8_t*) ops[1]));
        Register *input1 = this->reg.getRegister(*((uint8_t*) ops[2]));
        // store the immediate value in a local register.
        Register input2 = Register();
        input2.write(*((int32_t*) ops[3]));
        switch(op) {
            case(Operations::ADDI):
                dest->write<int>(input1->read<int>()+input2.read<int>());
                break;
            //TODO implement these ops.
            default:
                //TODO some sort of exception
                throw "NOT IMPLEMENTED";
                break;
        }
    }
    pc+=64;
    this->reg.getRegister(PC)->write<unsigned long>(pc);
}
