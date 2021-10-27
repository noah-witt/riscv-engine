#include <boost/test/unit_test.hpp>
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

BOOST_AUTO_TEST_SUITE(alu_tests)
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
    BOOST_ASSERT(a.getReg()->getRegister(PC)->read<unsigned long>()==INSTRUCTION_LENGTH);
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==37);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==27);
    BOOST_ASSERT(a.getReg()->getRegister(PC)->read<unsigned long>()==INSTRUCTION_LENGTH*2);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==160);
    BOOST_ASSERT(a.getReg()->getRegister(PC)->read<unsigned long>()==INSTRUCTION_LENGTH*3);
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
    loc+=INSTRUCTION_LENGTH;
    BOOST_ASSERT(mem->read<char>(loc).payload=='a');
    loc+=INSTRUCTION_LENGTH;
    BOOST_ASSERT(mem->read<char>(loc).payload=='b');
    loc+=INSTRUCTION_LENGTH;
    BOOST_ASSERT(mem->read<char>(loc).payload=='c');
    loc+=INSTRUCTION_LENGTH;
    BOOST_ASSERT(mem->read<char>(loc).payload=='1');
    loc+=INSTRUCTION_LENGTH;
    BOOST_ASSERT(mem->read<char>(loc).payload=='2');
    loc+=INSTRUCTION_LENGTH;
    BOOST_ASSERT(mem->read<char>(loc).payload=='3');
    loc+=INSTRUCTION_LENGTH;
    BOOST_ASSERT(mem->read<char>(loc).payload=='\0');
    // eight commands are tested.
    BOOST_ASSERT(mem->stringFromMemory(a.getReg()->getRegister(PC)->read<unsigned long>()+INSTRUCTION_LENGTH)=="abc123");
    BOOST_ASSERT(mem->stringFromMemory<2>(a.getReg()->getRegister(PC)->read<unsigned long>()+INSTRUCTION_LENGTH)=="ab");
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
    Program program = Program("LD t0, offsetSize\n ADDI t0, t0, testVal\n LD t0,0(t0)\n \nhalt\ntestVal: .dword 100 \n .dword 350\noffsetSize: .dword 8");
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    a.getReg()->getRegister(0);
    a.getReg()->getRegister(0)->write<unsigned long>(0);
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<unsigned long>()==8);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<unsigned long>()==40);
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<unsigned long>()==350);
}

BOOST_AUTO_TEST_CASE(loop_test) {
    Program program = Program("LD t0, offsetSize\n ADDI t0, t0, testVal\n LD t0,0(t0)\n \nhalt\ntestVal: .dword 100 \n .dword 350\noffsetSize: .dword 8");
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
    std::ifstream ifs("./test_files/test_alu_a.S");
    Program program = Program(ifs);
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    //first jump is at 8
    a.getReg()->getRegister(PC)->write<unsigned long>(8);
    BOOST_LOG_TRIVIAL(debug) << "Now starting loop";
    a.loop(10);
    BOOST_ASSERT(a.getReg()->getRegister(7)->read<int>()==700);
    // now do section b
    a.getReg()->getRegister(PC)->write<unsigned long>(INSTRUCTION_LENGTH*2);
    BOOST_LOG_TRIVIAL(debug) << "starting loop two";
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(10)->read<int>()==800);
    BOOST_ASSERT(a.getReg()->getRegister(11)->read<int>()==-800);
    // test branching instructions.
    a.getReg()->getRegister(PC)->write<unsigned long>(INSTRUCTION_LENGTH*3);
    BOOST_LOG_TRIVIAL(debug) << "starting loop two";
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==990);
    a.getReg()->getRegister(PC)->write<unsigned long>(INSTRUCTION_LENGTH*4);
    BOOST_LOG_TRIVIAL(debug) << "starting loop two testing branch";
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==991);
    // sectD tests
    a.getReg()->getRegister(PC)->write<unsigned long>(INSTRUCTION_LENGTH*5);
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==2001);
    a.getReg()->getRegister(PC)->write<unsigned long>(INSTRUCTION_LENGTH*6);
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==200);
    a.getReg()->getRegister(PC)->write<unsigned long>(INSTRUCTION_LENGTH*7);
    a.loop();
    BOOST_ASSERT(a.getReg()->getRegister(13)->read<int>()==200);
    // section e is input testing
    std::string e_input_data = "input example\n";
    std::stringstream e_input(e_input_data);
    std::stringstream e_output;
    a.getReg()->getRegister(PC)->write<unsigned long>(INSTRUCTION_LENGTH*8);
    a.loop(e_input, e_output);
    std::string res = e_output.str();
    BOOST_LOG_TRIVIAL(debug) <<"the output produced by the code: "<< res;
    BOOST_ASSERT(res=="demo output abc123input example");
}

BOOST_AUTO_TEST_CASE(even_odd_test) {
    // first validate that from a file works ok
    std::ifstream ifs("./test_files/test_alu_b.S");
    Program program = Program(ifs);
    alu a;
    Memory *mem = a.getMem();
    program.toMemory(mem);
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    std::string input_data = "2\n5\n6\n7\n-1\n";
    std::stringstream input(input_data);
    std::stringstream output;
    a.loop(input, output, 150);
    std::string res = output.str();
    BOOST_LOG_TRIVIAL(debug) <<"the output produced by the code:"<< res;
    BOOST_ASSERT(res=="Enter a number of -1 if done even\nEnter a number of -1 if done odd\nEnter a number of -1 if done even\nEnter a number of -1 if done odd\nEnter a number of -1 if done bye");
    a.getReg()->getRegister(PC)->write<unsigned long>(INSTRUCTION_LENGTH*1);
    std::stringstream input_2("");
    std::stringstream output_2;
    a.loop(input_2, output_2);
    res = output_2.str();
    BOOST_LOG_TRIVIAL(debug) <<"test 2:"<< res;
    BOOST_ASSERT(res=="FizzBuzz 15    14    13   Fizz 12    11   Buzz 10   Fizz 9    8    7   Fizz 6   Buzz 5    4   Fizz 3    2    1   ");
}

BOOST_AUTO_TEST_SUITE_END()
