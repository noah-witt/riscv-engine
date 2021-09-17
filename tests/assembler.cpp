#define BOOST_TEST_MODULE memory_test

#include <boost/test/included/unit_test.hpp>
#include "../assemble.hpp"

BOOST_AUTO_TEST_CASE(one_command_test) {
    Program program = Program("ADD ra ra ra");
    Memory mem;
    BOOST_ASSERT(mem.read<unsigned long>(0).payload == 0);
    program.toMemory(&mem);
    // THIS SHOULD BE FALSE.
    BOOST_ASSERT(mem.read<unsigned long>(0).payload == 0);
}