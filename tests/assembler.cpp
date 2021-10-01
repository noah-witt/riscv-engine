#define BOOST_TEST_MODULE assembler_test

#include <boost/test/included/unit_test.hpp>
#include "../assemble.hpp"
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>


BOOST_AUTO_TEST_CASE(symbol_table_test) {
    SymbolTable s = SymbolTable();
    s.insert("aaa", 123, 64);
    s.insert("BBB", 156, 64);
    SymbolTableFindResult temp = s.find("AAAA");
    BOOST_ASSERT(temp.found == 0);
    temp = s.find("aaa");
    BOOST_ASSERT(temp.found == 1);
    BOOST_ASSERT(!s.insert("aaa", 929, 64) == 1);
}

BOOST_AUTO_TEST_CASE(INSTRUCTION_TEST) {
    SymbolTable s = SymbolTable();
    Instruction i = Instruction("ADD t0, t1, t2", &s, 0);
    Register instruction;
    instruction.write<unsigned long>(i.getInstruction().values[0]);
    BOOST_LOG_TRIVIAL(debug) << instruction.read<unsigned long>();
    std::array<void *, 4> ops = instruction.readInstruction<uint16_t, uint16_t, uint16_t, uint16_t>(); 
    BOOST_ASSERT(*(uint16_t *)ops[0] == 0);
    BOOST_ASSERT(*(uint16_t *)ops[1] == 5);
    BOOST_ASSERT(*(uint16_t *)ops[2] == 6);
    BOOST_ASSERT(*(uint16_t *)ops[3] == 7);
}

BOOST_AUTO_TEST_CASE(command_test_one) {
    Program program = Program("ADD t0, t1, t2 \nADD t0, t1, t2");
    Memory mem;
    BOOST_ASSERT(mem.read<unsigned long>(0).payload == 0);
    program.toMemory(&mem);
    BOOST_ASSERT(mem.read<unsigned long>(0).payload == 1970350607106048);
    BOOST_ASSERT(mem.read<unsigned long>(64).payload == 1970350607106048);
    BOOST_ASSERT(mem.read<unsigned long>(128).payload == 0);
    BOOST_ASSERT(mem.read<unsigned long>(192).payload == 0);
}

BOOST_AUTO_TEST_CASE(command_test_two) {
    Program program = Program("ADDI t0, t1, 127 \nADD t0, t1, t2");
    Memory mem;
    BOOST_ASSERT(mem.read<unsigned long>(0).payload == 0);
    program.toMemory(&mem);
    readResult<unsigned long> addiReadResult = mem.read<unsigned long>(0); // read the addi command.
    Register addi;
    addi.write<unsigned long>(addiReadResult.payload);
    //BOOST_ASSERT(addi.read<uint16_t,0>() == (uint16_t)Operations::ADDI);
    //BOOST_ASSERT(addi.read<uint16_t,2>() == 5);
    //BOOST_ASSERT(addi.read<uint16_t,4>() == 7);
    //BOOST_ASSERT(mem.read<unsigned long>(64).payload == 1970350607106048);
    //BOOST_ASSERT(mem.read<unsigned long>(128).payload == 0);
    //BOOST_ASSERT(mem.read<unsigned long>(192).payload == 0);
}

