#define BOOST_TEST_MODULE register_test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#include "../memory.hpp"
#include "../memory.t.hpp"
#include "../registers.hpp"

BOOST_AUTO_TEST_CASE(byte_write_test) {
	Register a = Register();
	a.writeByte(0xa);
    BOOST_ASSERT(0xa == a.readByte().payload);
    BOOST_ASSERT(0xb != a.readByte().payload);
    a.writeByte(0xc);
    BOOST_ASSERT(0xb != a.readByte().payload);
    BOOST_ASSERT(0xc == a.readByte().payload);
    BOOST_ASSERT(0xc == a.readLong().payload);
    BOOST_ASSERT(0xc == a.readShort().payload);
    BOOST_ASSERT(0xc == a.read().payload);
}

BOOST_AUTO_TEST_CASE(short_write_test) {
	Register a = Register();
	a.writeShort(0xaab);
    BOOST_ASSERT(0xaab == a.readShort().payload);
    BOOST_ASSERT(0xaac != a.readShort().payload);
    a.writeShort(0xaad);
    BOOST_ASSERT(0xb != a.readShort().payload);
    BOOST_ASSERT(0xaad == a.readShort().payload);
}

BOOST_AUTO_TEST_CASE(write_test) {
	Register a = Register();
	a.write(INT32_MAX-10);
    BOOST_ASSERT(INT32_MAX-10 == a.read().payload);
    BOOST_ASSERT(INT32_MAX-11 != a.read().payload);
    a.write(INT32_MAX-9);
    BOOST_ASSERT(INT32_MAX-10 != a.read().payload);
    BOOST_ASSERT(INT32_MAX-9 == a.read().payload);
}

BOOST_AUTO_TEST_CASE(long_write_test) {
	Register a = Register();
	a.writeLong(LONG_MAX-10);
    BOOST_ASSERT(LONG_MAX-10 == a.readLong().payload);
    BOOST_ASSERT(LONG_MAX-11 != a.readLong().payload);
    a.writeLong(LONG_MAX-9);
    BOOST_ASSERT(LONG_MAX-10 != a.readLong().payload);
    BOOST_ASSERT(LONG_MAX-9 == a.readLong().payload);
}


// this acts to test the read functionality of reading to lower order bytes.
BOOST_AUTO_TEST_CASE(lower_order_byte_test) {
	Register a = Register();
    // 64 bit = 8 byte
    // one hex character is half a byte
    // 16 digits here
	a.writeLong(0x0123456789abcdef);
    BOOST_ASSERT(0x0123456789abcdef == a.readLong().payload);
    BOOST_ASSERT(0x89abcdef == a.read().payload);
    BOOST_ASSERT(0xcdef == a.readShort().payload);
    BOOST_ASSERT(0xef == a.readByte().payload);
}

/*BOOST_AUTO_TEST_CASE(zero_reg_test) {
*    Register a = zeroRegister();
*    BOOST_ASSERT(0 == a.readLong().payload);
*    BOOST_ASSERT(0x0 == a.read().payload);
*    BOOST_ASSERT(0x0 == a.readShort().payload);
*    BOOST_ASSERT(0x0 == a.readByte().payload);
*    BOOST_ASSERT(!a.write(0x1123));
*    BOOST_ASSERT(0x0 == a.readLong().payload);
*}
*/
