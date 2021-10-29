#include <boost/test/unit_test.hpp>
#include "../memory.hpp"
#include "../memory.t.hpp"

BOOST_AUTO_TEST_SUITE(memory)
BOOST_AUTO_TEST_CASE(first_write_test) {
	Memory a = Memory();
	a.writeByte(0,0xa);
	BOOST_CHECK_EQUAL(0xa, a.readByte(0).payload);
}



BOOST_AUTO_TEST_CASE(write_random_byte_test) {
	//handle byte writes and reads.
	for(int i =0; i< 10; i++) {
		Memory a = Memory();
		unsigned char num = rand()%UCHAR_MAX;
		unsigned long address = rand()%MAXMEMORY;
		//printf("%p\t%X\n", address, num);
		a.writeByte(address,num);
		//a.write<int>(address, num);
		BOOST_CHECK_EQUAL(num, a.readByte(address).payload);
	}
}

BOOST_AUTO_TEST_CASE(write_random_int_test) {
	//handle int writes and reads.
	Memory a = Memory();
	for(int i =0; i< 10; i++) {
		unsigned int num = rand()%UINT_MAX;
		unsigned long address = rand()%MAXMEMORY;
		//printf("%p\t%X\n", address, num);
		a.write<unsigned int>(address,num);
		//a.write<int>(address, num);
		BOOST_CHECK_EQUAL(num, a.read<unsigned int>(address).payload);
	}
}

BOOST_AUTO_TEST_CASE(verify_bounds_checking) {
	Memory a = Memory();
	a.read<int>(MAXMEMORY).valid;
	BOOST_CHECK_EQUAL(a.read<int>(MAXMEMORY).valid, false);
	BOOST_CHECK_EQUAL(a.read<int>(MAXMEMORY+100).valid, false);
	BOOST_CHECK_EQUAL(a.read<int>(0).valid, true);
	BOOST_CHECK_EQUAL(a.read<int>(MAXMEMORY-sizeof(int)).valid, true);
	BOOST_CHECK_EQUAL(a.read<int>(MAXMEMORY/2).valid, true);
}

BOOST_AUTO_TEST_CASE(verify_byte_op) {
	Memory a = Memory();
	a.writeByte(0, 'a');
	a.writeByte(1, 'b');
	a.writeByte(3, 'd');
	a.writeByte(PAGESIZE-2, 'a');
	a.writeByte(PAGESIZE-1, 'b');
	a.writeByte(PAGESIZE, 'c');
	a.writeByte(PAGESIZE+1, 'd');
	a.writeByte(PAGESIZE+2, 'e');
	BOOST_CHECK_EQUAL(a.readByte(0).payload, 'a');
	BOOST_CHECK_EQUAL(a.readByte(1).payload, 'b');
	BOOST_CHECK_EQUAL(a.readByte(3).payload=='a', false);
}

BOOST_AUTO_TEST_CASE(byte_op_behavior) {
	Memory a = Memory();
	a.write<int>(0, 20);
	BOOST_CHECK_EQUAL(a.readByte(0).payload, 20);
	BOOST_CHECK_EQUAL(a.readByte(1).payload, 0);
	BOOST_CHECK_EQUAL(a.readByte(2).payload, 0);
	BOOST_CHECK_EQUAL(a.readByte(3).payload, 0);
}

BOOST_AUTO_TEST_CASE(byte_op_boundary_behavior) {
	Memory a = Memory();
	a.write<int>(PAGESIZE-(sizeof(int))+1, 20);
	BOOST_CHECK_EQUAL(a.readByte(PAGESIZE-(sizeof(int))+1).payload, 20);
	BOOST_CHECK_EQUAL(a.readByte(PAGESIZE-(sizeof(int))+2).payload, 0);
	BOOST_CHECK_EQUAL(a.readByte(PAGESIZE-(sizeof(int))+3).payload, 0);
	BOOST_CHECK_EQUAL(a.readByte(PAGESIZE-(sizeof(int))+4).payload, 0);
}

BOOST_AUTO_TEST_CASE(verify_op_on_split) {
	Memory a = Memory();
	a.write<int>(PAGESIZE-(sizeof(int))+1, 1234567891);
	BOOST_CHECK_EQUAL(a.read<int>(PAGESIZE-(sizeof(int))+1).valid, true);
	BOOST_CHECK_EQUAL(a.read<int>(PAGESIZE-(sizeof(int))+1).payload, 1234567891);
	a.write<long>((2*PAGESIZE)-(sizeof(int))+1, 1234567891234567);
	BOOST_CHECK_EQUAL(a.read<long>((2*PAGESIZE)-(sizeof(int))+1).payload, 1234567891234567);
	BOOST_CHECK_EQUAL(a.read<long>((2*PAGESIZE)-(sizeof(int))+1).valid, true);
}


BOOST_AUTO_TEST_CASE(write_full_int_test) {
	//fully write the first 3 pages and then the last 3 pages.
	Memory a = Memory();
	for(int i =0; i< 3*PAGESIZE; i+=sizeof(int)) {
		unsigned int num = rand()%UINT_MAX;
		a.write<unsigned int>(i,num);
		BOOST_CHECK_EQUAL(num, a.read<unsigned int>(i).payload);
	}

	for(int i =MAXMEMORY-(3*PAGESIZE); i<MAXMEMORY-sizeof(int); i+=sizeof(int)) {
		unsigned int num = rand()%UINT_MAX;
		a.write<unsigned int>(i,num);
		BOOST_CHECK_EQUAL(num, a.read<unsigned int>(i).payload);
	}
}

BOOST_AUTO_TEST_SUITE_END()

