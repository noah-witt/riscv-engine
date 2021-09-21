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
    Register instruction = i.getInstruction();
    BOOST_LOG_TRIVIAL(debug) << instruction.readLong().payload;
    BOOST_ASSERT(instruction.readInstruction()[0] == 0);
    BOOST_ASSERT(instruction.readInstruction()[1] == 5);
    BOOST_ASSERT(instruction.readInstruction()[2] == 6);
    BOOST_ASSERT(instruction.readInstruction()[3] == 7);
}

BOOST_AUTO_TEST_CASE(one_command_test) {
    Program program = Program("ADD t0, t1, t2 \nADD t0, t1, t2");
    Memory mem;
    BOOST_ASSERT(mem.read<unsigned long>(0).payload == 0);
    program.toMemory(&mem);
    BOOST_ASSERT(mem.read<unsigned long>(0).payload == 1970350607106048);
    BOOST_ASSERT(mem.read<unsigned long>(64).payload == 1970350607106048);
    BOOST_ASSERT(mem.read<unsigned long>(128).payload == 0);
    BOOST_ASSERT(mem.read<unsigned long>(192).payload == 0);
}
// TODO more tests 
