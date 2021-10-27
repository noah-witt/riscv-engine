#include <boost/test/unit_test.hpp>
#include "../memory.hpp"
#include "../memory.t.hpp"
#include "../registers.hpp"
#include "../registers.t.hpp"
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>

BOOST_AUTO_TEST_SUITE(registers)
BOOST_AUTO_TEST_CASE(int_write_test) {
	Register a = Register();
	a.write<unsigned int>(0xa);
    BOOST_ASSERT(0xa == a.read<unsigned char>());
    BOOST_ASSERT(0xb != a.read<unsigned char>());
    a.write<unsigned int>(0xc);
    BOOST_ASSERT(0xb != a.read<unsigned char>());
    BOOST_ASSERT(0xc == a.read<unsigned char>());
    BOOST_ASSERT(0xc == a.read<unsigned long>());
    BOOST_ASSERT(0xc == a.read<unsigned short>());
}

BOOST_AUTO_TEST_CASE(short_write_test) {
	Register a = Register();
	a.write<unsigned short>(0xaab);
    BOOST_ASSERT(0xaab == a.read<unsigned short>());
    BOOST_ASSERT(0xaac != a.read<unsigned short>());
    a.write<unsigned short>(0xaad);
    BOOST_ASSERT(0xb != a.read<unsigned short>());
    BOOST_ASSERT(0xaad == a.read<unsigned short>());
}

BOOST_AUTO_TEST_CASE(zero_reg_test) {
    Register a = zeroRegister();
    a.write(0U);
    BOOST_ASSERT(0 == a.read<unsigned long>());
    BOOST_ASSERT(0 == a.read<unsigned int>());
    BOOST_ASSERT(0 == a.read<int>());
    BOOST_ASSERT(0 == a.read<short>());
    a.write(0x1123);
    BOOST_ASSERT(0 == a.read<unsigned long>());
}
BOOST_AUTO_TEST_SUITE_END()

