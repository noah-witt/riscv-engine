#define BOOST_TEST_MODULE register_test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#include "../memory.hpp"
#include "../memory.t.hpp"
#include "../registers.hpp"
#include "../registers.t.hpp"

BOOST_AUTO_TEST_CASE(byte_write_test) {
	Register a = Register();
	a.writeLower<unsigned char>(0xa);
    BOOST_ASSERT(0xa == *a.readLower<unsigned char>());
    BOOST_ASSERT(0xb != *a.readLower<unsigned char>());
    a.writeLower<unsigned char>(0xc);
    BOOST_ASSERT(0xb != *a.readLower<unsigned char>());
    BOOST_ASSERT(0xc == *a.readLower<unsigned char>());
    BOOST_ASSERT(0xc == *a.readLower<unsigned long>());
    BOOST_ASSERT(0xc == *a.readLower<unsigned short>());
}

BOOST_AUTO_TEST_CASE(short_write_test) {
	Register a = Register();
	a.write<ushort>(0xaab);
    BOOST_ASSERT(0xaab == *a.readLower<unsigned short>());
    BOOST_ASSERT(0xaac != *a.readLower<unsigned short>());
    a.write<ushort>(0xaad);
    BOOST_ASSERT(0xb != *a.readLower<unsigned short>());
    BOOST_ASSERT(0xaad == *a.readLower<unsigned short>());
}

BOOST_AUTO_TEST_CASE(zero_reg_test) {
    Register a = zeroRegister();
    BOOST_ASSERT(0 == *a.read<unsigned long>());
    BOOST_ASSERT(0 == *a.read<unsigned int>());
    BOOST_ASSERT(0 == *a.read<int>());
    BOOST_ASSERT(0 == *a.read<short>());
    a.write(0x1123);
    BOOST_ASSERT(0 == *a.read<unsigned long>());
}

