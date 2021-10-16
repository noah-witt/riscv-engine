#define BOOST_TEST_MODULE assembler_test

#include <boost/test/included/unit_test.hpp>
#include "../assemble.hpp"
#include "../alu.hpp"
#include <iostream>
#include <ostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>

BOOST_AUTO_TEST_CASE(alu_step_basic_test) {
    Program program = Program("ADD t0, t1, t2\nSUB t0, t1, t2\nMUL t0, t1, t2\n HALT\n\rADDI t0, t1, 100\nADDI t0, t1, -50\nLUI t1, 150\nADDI t0, t1, -50\nADDI t0, t0, 200\n.word 500\n.asciiz \"abc123\"");
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
}

BOOST_AUTO_TEST_CASE(label_use_test) {
    Program program = Program("LD t0, testVal\nhalt\ntestVal: .dword 100");
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    a.getReg()->getRegister(0);
    a.getReg()->getRegister(0)->write<unsigned long>(0);
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<unsigned long>()==100);
}

BOOST_AUTO_TEST_CASE(complex_label_test) {
    Program program = Program("LD t0, offsetSize\n ADDI t0, t0, testVal\n LD t0,0(t0)\n \nhalt\ntestVal: .dword 100 \n .dword 350\noffsetSize: .dword 64");
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    a.getReg()->getRegister(0);
    a.getReg()->getRegister(0)->write<unsigned long>(0);
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<unsigned long>()==64);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<unsigned long>()==320);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<unsigned long>()==350);
}

BOOST_AUTO_TEST_CASE(loop_test) {
    Program program = Program("LD t0, offsetSize\n ADDI t0, t0, testVal\n LD t0,0(t0)\n \nhalt\ntestVal: .dword 100 \n .dword 350\noffsetSize: .dword 64");
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<unsigned long>()==350);
}

BOOST_AUTO_TEST_CASE(print_test) {
    Program program = Program("PRINT 0, str\n # abc 123\n HALT\n PRINT 0, strb\n PRINT 0, strc \nHALT\n\rstr: .asciiz \"abc123\"\nstrb: .asciiz \"def456\"\n.dword 0\n.dword 100\nstrc: .asciiz \"string test for c. abc 123 \"");
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    AluStepResult res = a.step();
    BOOST_ASSERT(res.printStr==true);
    BOOST_ASSERT(res.printStrValue=="abc123");
    res = a.step();
    BOOST_ASSERT(res.printStr==false);
    res = a.step();
    BOOST_ASSERT(res.printStr==true);
    BOOST_ASSERT(res.printStrValue=="def456");
    res = a.step();
    BOOST_ASSERT(res.printStr==true);
    BOOST_ASSERT(res.printStrValue=="string test for c. abc 123 ");
}

// this section reads from a file, tests branch, and jump instructions.
BOOST_AUTO_TEST_CASE(jump_tests) {
    // first validate that from a file works ok
    std::ifstream ifs("./tests/test_alu_a.S");
    Program program = Program(ifs);
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    //first jump is at 64
    a.getReg()->getRegister(PC)->write<unsigned long>(64);
    BOOST_LOG_TRIVIAL(debug) << "Now starting loop";
    a.loop(10);
    BOOST_ASSERT(a.getReg()->getRegister(7)->read<int>()==700);
    // now do section b
    a.getReg()->getRegister(PC)->write<unsigned long>(128);
    BOOST_LOG_TRIVIAL(debug) << "starting loop two";
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(10)->read<int>()==800);
    BOOST_ASSERT(a.getReg()->getRegister(11)->read<int>()==-800);
    // test branching instructions.
    a.getReg()->getRegister(PC)->write<unsigned long>(192);
    BOOST_LOG_TRIVIAL(debug) << "starting loop two";
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==990);
    a.getReg()->getRegister(PC)->write<unsigned long>(256);
    BOOST_LOG_TRIVIAL(debug) << "starting loop two testing branch";
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==991);
    // sectD tests
    a.getReg()->getRegister(PC)->write<unsigned long>(320);
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==2001);
    a.getReg()->getRegister(PC)->write<unsigned long>(384);
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==200);
    a.getReg()->getRegister(PC)->write<unsigned long>(448);
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==200);
    // section e is input testing
    std::string e_input_data = "input example\n";
    std::stringstream e_input(e_input_data);
    std::stringstream e_output;
    a.getReg()->getRegister(PC)->write<unsigned long>(512);
    a.loop(e_input, e_output);
    std::string res = e_output.str();
    BOOST_LOG_TRIVIAL(debug) <<"the output produced by the code: "<< res;
    BOOST_ASSERT(res=="demo output abc123input example");
    std::string e_input_data_2 = "997\n";
    std::stringstream e_input_2(e_input_data_2);
    std::stringstream e_output_2;
    a.loop(e_input_2, e_output_2);
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==150);
    a.loop(e_input_2, e_output_2);
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==997);
    BOOST_ASSERT(false==true); // a temp expression to force this to fail at the end.
}
