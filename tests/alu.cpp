#define BOOST_TEST_MODULE assembler_test

#include <boost/test/included/unit_test.hpp>
#include "../assemble.hpp"
#include "../alu.hpp"
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>

BOOST_AUTO_TEST_CASE(alu_step_basic_test) {
    Program program = Program("ADD t0, t1, t2\nSUB t0, t1, t2\nMUL t0, t1, t2\n HALT\n\rADDI t0, t1, 100\nADDI t0, t1, -50\nLUI t1, 150\nADDI t0, t1, -50\nADDI t0, t0, 200\n.word 500\n.asciz \"abc123\"");
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    BOOST_LOG_TRIVIAL(debug) << "pre reg setup";
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    a.getReg()->getRegister(6)->write<int>(32);
    a.getReg()->getRegister(7)->write<int>(5);
    BOOST_ASSERT(a.getMem()->read<unsigned long>(0).payload == mem->read<unsigned long>(0).payload);
    BOOST_LOG_TRIVIAL(debug) << "memory state at step 1 before run: "<<a.getMem()->read<unsigned long>(0).payload;
    Register cmd;
    BOOST_LOG_TRIVIAL(debug) << "memory used for execution "<<mem;
    cmd.write<unsigned long>(mem->read<unsigned long>(0).payload);
    std::array<void *, 4> cmdParts = cmd.readInstruction<uint16_t, uint16_t, uint16_t, uint16_t>();
    BOOST_ASSERT(*(uint16_t*)cmdParts[0]==0);
    BOOST_ASSERT(*(uint16_t*)cmdParts[1]==5);
    BOOST_ASSERT(*(uint16_t*)cmdParts[2]==6);
    BOOST_ASSERT(*(uint16_t*)cmdParts[3]==7);
    BOOST_ASSERT(a.getReg()->getRegister(PC)->read<unsigned long>()==0);
    BOOST_ASSERT(a.getReg()->getRegister(6)->read<unsigned long>()==32);
    BOOST_ASSERT(a.getReg()->getRegister(7)->read<unsigned long>()==5);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(PC)->read<unsigned long>()==64);
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==37);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==27);
    BOOST_ASSERT(a.getReg()->getRegister(PC)->read<unsigned long>()==128);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==160);
    BOOST_ASSERT(a.getReg()->getRegister(PC)->read<unsigned long>()==192);
    BOOST_ASSERT(a.step().halt==true);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==132);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==-18);
    // LUI instruction that changes register 6 to 150.
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(6)->read<int>()==150);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==100);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==300);
    ulong loc = a.getReg()->getRegister(PC)->read<unsigned long>();
    // check the memory info that is provided.
    BOOST_ASSERT(mem->read<int>(loc).payload==500);
    loc+=64;
    BOOST_ASSERT(mem->read<char>(loc).payload=='a');
    loc+=64;
    BOOST_ASSERT(mem->read<char>(loc).payload=='b');
    loc+=64;
    BOOST_ASSERT(mem->read<char>(loc).payload=='c');
    loc+=64;
    BOOST_ASSERT(mem->read<char>(loc).payload=='1');
    loc+=64;
    BOOST_ASSERT(mem->read<char>(loc).payload=='2');
    loc+=64;
    BOOST_ASSERT(mem->read<char>(loc).payload=='3');
    loc+=64;
    BOOST_ASSERT(mem->read<char>(loc).payload=='\0');
    // eight commands are tested.
    BOOST_ASSERT(mem->stringFromMemory(a.getReg()->getRegister(PC)->read<unsigned long>()+64)=="abc123");
    BOOST_ASSERT(mem->stringFromMemory<2>(a.getReg()->getRegister(PC)->read<unsigned long>()+64)=="ab");
    BOOST_ASSERT(false==true); // a temp expression to force this to fail at the end.
}

