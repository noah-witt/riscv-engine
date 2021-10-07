#include "./alu.hpp"
#include "./registers.hpp"
#include "./assemble.hpp"
#include "./registers.t.hpp"
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

AluStepResult alu::step() {
    AluStepResult result;
    unsigned long pc = this->reg.getRegister(PC)->read<unsigned long>();
    BOOST_LOG_TRIVIAL(debug) << "stepping with memory " << &this->mem << ", and pc " << pc;
    Register operation;
    operation.write<unsigned long>(this->mem.read<unsigned long>(pc).payload);
    // TODO read at location and then execute
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
                break;
            case(Operations::SUBW):
                dest->write<long>(input1->read<long>()-input2->read<long>());
                break;
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
                break;
            case(Operations::DIVW):
                dest->write<long>(input1->read<long>()/input2->read<long>());
                break;
            case(Operations::DIVUW):
                dest->write<ulong>(input1->read<ulong>()/input2->read<ulong>());
                break;
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
        input2.write<int>(*((int32_t*) ops[3]));
        // this results in the operating being similar to their non immediate version.
        switch(op) {
            case(Operations::ADDI):
                dest->write<int>(input1->read<int>()+input2.read<int>());
                break;
            case(Operations::SLTI):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SLLI):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SRLI):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::XORI):
                //FIXME implement
                dest->write<uint>(input1->read<uint>() ^ input2.read<uint>());
                break;
            case(Operations::SRAI):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::ADDIW):
                dest->write<long>(input1->read<long>()+input2.read<int>());
                break;
            case(Operations::SLLIW):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SRLIW):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::SRAIW):
                //FIXME implement
                throw "NOT IMPLEMENTED";
            case(Operations::JALR):
                // an instruction that performs two operations.
                dest->write<long>(this->reg.getRegister(PC)->read<ulong>()+INSTRUCTION_LENGTH);
                this->reg.getRegister(PC)->write<ulong>(input1->read<int>()+input2.read<int>());
                break;
            case(Operations::BEQ):
                if(dest->read<unsigned long>()==input1->read<unsigned long>()) {
                    //-INSTRUCTION_LENGTH because the instruction length is added for all commands.
                    this->reg.getRegister(PC)->write<ulong>(this->reg.getRegister(PC)->read<ulong>()+input2.read<int>()-INSTRUCTION_LENGTH);
                }
                break;
            case(Operations::BNE):
                if(dest->read<unsigned long>()!=input1->read<unsigned long>()) {
                    //-INSTRUCTION_LENGTH because the instruction length is added for all commands.
                    this->reg.getRegister(PC)->write<ulong>(this->reg.getRegister(PC)->read<ulong>()+input2.read<int>()-INSTRUCTION_LENGTH);
                }
                break;
            case(Operations::BLT):
                if(dest->read<long>()<input1->read<long>()) {
                    //-INSTRUCTION_LENGTH because the instruction length is added for all commands.
                    this->reg.getRegister(PC)->write<ulong>(this->reg.getRegister(PC)->read<ulong>()+input2.read<int>()-INSTRUCTION_LENGTH);
                }
                break;
            case(Operations::BGE):
                if(dest->read<long>()>=input1->read<long>()) {
                    //-INSTRUCTION_LENGTH because the instruction length is added for all commands.
                    this->reg.getRegister(PC)->write<ulong>(this->reg.getRegister(PC)->read<ulong>()+input2.read<int>()-INSTRUCTION_LENGTH);
                }
                break;
            case(Operations::BLTU):
                if(dest->read<ulong>()<input1->read<ulong>()) {
                    //-INSTRUCTION_LENGTH because the instruction length is added for all commands.
                    this->reg.getRegister(PC)->write<ulong>(this->reg.getRegister(PC)->read<ulong>()+input2.read<int>()-INSTRUCTION_LENGTH);
                }
                break;
            case(Operations::BGEU):
                if(dest->read<ulong>()>=input1->read<ulong>()) {
                    //-INSTRUCTION_LENGTH because the instruction length is added for all commands.
                    this->reg.getRegister(PC)->write<ulong>(this->reg.getRegister(PC)->read<ulong>()+input2.read<int>()-INSTRUCTION_LENGTH);
                }
                break;
            //TODO implement these ops.
            default:
                //TODO some sort of exception
                throw "NOT IMPLEMENTED";
                break;
        }
    } else if(op>=Operations::LB && op<=Operations::SD) {
        // These are memory operatins that have to operate on the memory that is available.
        // JAL is not a memory operation
        std::array<void *, 4> ops = operation.readInstruction<uint16_t, uint8_t, uint8_t, int32_t>();
        BOOST_LOG_TRIVIAL(debug) << "processing memory operation "<<*((uint8_t*) ops[1])<<" "<<*((uint8_t*) ops[2])<<" "<<*((int32_t*) ops[3]);
        Register *target = this->reg.getRegister(*((uint8_t*) ops[1])); //destination or source
        Register *offsetFrom = this->reg.getRegister(*((uint8_t*) ops[2]));
        // the memory location is the offsetFromRegister+offset
        ulong memLocation = offsetFrom->read<long>()+*((int32_t*) ops[3]);
        switch(op) {
            // load instructions
            case(Operations::LB):
                target->write<char>(this->mem.read<char>(memLocation).payload);
                break;
            case(Operations::LH):
                target->write<short>(this->mem.read<short>(memLocation).payload);
                break;
            case(Operations::LW):
                target->write<int>(this->mem.read<int>(memLocation).payload);
                break;
            case(Operations::LWU):
                target->write<unsigned int>(this->mem.read<unsigned int>(memLocation).payload);
                break;
            case(Operations::LD):
                target->write<long>(this->mem.read<long>(memLocation).payload);
                break;
            case(Operations::LBU):
                target->write<unsigned char>(this->mem.read<unsigned char>(memLocation).payload);
                break;
            case(Operations::LHU):
                target->write<unsigned short>(this->mem.read<unsigned short>(memLocation).payload);
                break;
            // store instructions
            case(Operations::SB):
                this->mem.write<char>(memLocation, target->read<char>());
                break;
            case(Operations::SH):
                this->mem.write<short>(memLocation, target->read<short>());
                break;
            case(Operations::SW):
                this->mem.write<int>(memLocation, target->read<int>());
                break;
            case(Operations::SD):
                this->mem.write<long>(memLocation, target->read<long>());
                break;
            //JAL is in this range but is not a memory operation
            case(Operations::JAL): {
                target->write<ulong>(this->reg.getRegister(PC)->read<ulong>()+INSTRUCTION_LENGTH);
                long offsetForInst = *((int32_t*) ops[3]);
                //-INSTRUCTION_LENGTH because the instruction length is added for all commands.
                this->reg.getRegister(PC)->write<ulong>(this->reg.getRegister(PC)->read<ulong>()+offsetForInst-INSTRUCTION_LENGTH);
                break;
            }
            default:
                throw "NOT IMPLEMENTED";
                break;
        }
    } else if(op>=Operations::LUI && op<=Operations::AUIPC) {
        std::array<void *, 4>  ops = operation.readInstruction<uint16_t, uint8_t, int32_t, uint8_t>();
        Register *reg = this->reg.getRegister(*((uint8_t*) ops[1]));
        int immediateVal = *((int32_t*) ops[2]);
        // the third part is not used for now.
        switch(op) {
            case(Operations::LUI):
                reg->write<long>(immediateVal);
                break;
            case(Operations::AUIPC):
                reg->write<long>(immediateVal+this->reg.getRegister(PC)->read<ulong>());
                break;
            default:
                throw "NOT IMPLEMENTED";
                break;
        }
    }
    // custom operations
    else if(op==Operations::HALT) {
        // halt operation.
        result.halt = true;
    }
    // TODO add more custom operations.
    // print etc
    // request input
    else {
        throw "operation section code not implemented";
    }
    pc+=INSTRUCTION_LENGTH;
    this->reg.getRegister(PC)->write<unsigned long>(pc);
    return result;
}
