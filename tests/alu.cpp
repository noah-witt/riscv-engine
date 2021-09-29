#define BOOST_TEST_MODULE assembler_test

#include <boost/test/included/unit_test.hpp>
#include "../assemble.hpp"
#include "../alu.hpp"
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>

void tat(Memory *a) {
    a->read<int>(0);
    a->write<int>(0, 0);
}

BOOST_AUTO_TEST_CASE(alu_step_basic_test) {
    Program program = Program("ADD t0, t1, t2");
    alu a;
    Memory *mem = a.getMem();
    BOOST_LOG_TRIVIAL(debug) << "tat test";
    tat(mem);
    BOOST_LOG_TRIVIAL(debug) << "pre to mem";
    mem->read<int>(0);
    mem->write<unsigned long>(0, 123UL);
    program.toMemory(mem);
    BOOST_LOG_TRIVIAL(debug) << "post to mem";
    mem->read<int>(0);
    BOOST_LOG_TRIVIAL(debug) << "pre reg setup";
    a.getReg()->getRegister(PC)->write<unsigned long>(0);
    a.getReg()->getRegister(6)->write<int>(32);
    a.getReg()->getRegister(7)->write<int>(5);
    BOOST_LOG_TRIVIAL(debug) << "pre step";
    a.step();
    BOOST_ASSERT(a.getReg()->getRegister(5)->read<int>()==37);
    BOOST_ASSERT(a.getReg()->getRegister(PC)->read<unsigned long>()==64);
}

