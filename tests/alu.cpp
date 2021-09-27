#define BOOST_TEST_MODULE assembler_test

#include <boost/test/included/unit_test.hpp>
#include "../assemble.hpp"
#include "../alu.hpp"
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>


BOOST_AUTO_TEST_CASE(alu_step_basic_test) {
    Program program = Program("ADD t0, t1, t2");
    alu a = alu();
    //Memory *mem = &(a.mem);
    BOOST_LOG_TRIVIAL(debug) << "pre to mem";
    //program.toMemory(mem);
    //BOOST_LOG_TRIVIAL(debug) << "post to mem";
    //BOOST_LOG_TRIVIAL(debug) << "pre reg setup";
    //a.reg.getRegister(PC)->write<unsigned long>(0);
    //a.reg.getRegister(6)->write<int>(32);
    //a.reg.getRegister(7)->write<int>(5);
    //BOOST_LOG_TRIVIAL(debug) << "pre step";
    //a.step();
    //BOOST_ASSERT(a.reg.getRegister(5)->read<int>()==37);
    //BOOST_ASSERT(a.reg.getRegister(PC)->read<unsigned long>()==64);
}

