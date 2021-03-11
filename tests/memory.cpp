#define BOOST_TEST_MODULE memory_test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#include "../memory.hpp"
#include "../memory.t.hpp"


BOOST_AUTO_TEST_CASE(first_write_test) {
	Memory a = Memory();
	a.writeByte(0,0xa);
	BOOST_CHECK_EQUAL(0xa, a.readByte(0).payload);
}



BOOST_AUTO_TEST_CASE(write_random_byte_test) {
	//handle byte writes and reads.
	for(int i =0; i< 100; i++) {
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
	for(int i =0; i< 100; i++) {
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
	BOOST_CHECK_EQUAL(a.read<int>(MAXMEMORY).valid, false);
	BOOST_CHECK_EQUAL(a.read<int>(MAXMEMORY+100).valid, false);
	BOOST_CHECK_EQUAL(a.read<int>(0).valid, true);
	BOOST_CHECK_EQUAL(a.read<int>(MAXMEMORY-sizeof(int)).valid, true);
	BOOST_CHECK_EQUAL(a.read<int>(MAXMEMORY/2).valid, true);
}

BOOST_AUTO_TEST_CASE(write_full_int_test) {
	//fully write the first 3 pages and then the last 3 pages.
	Memory a = Memory();
	for(int i =0; i< 3*PAGESIZE; i++) {
		unsigned int num = rand()%UINT_MAX;
		//printf("%p\t%X\n", address, num);
		a.write<unsigned int>(i,num);
		//a.write<int>(address, num);
		BOOST_CHECK_EQUAL(num, a.read<unsigned int>(i).payload);
	}

	for(int i =MAXMEMORY-(3*PAGESIZE); i<MAXMEMORY; i++) {
		unsigned int num = rand()%UINT_MAX;
		//printf("%p\t%X\n", address, num);
		a.write<unsigned int>(i,num);
		//a.write<int>(address, num);
		BOOST_CHECK_EQUAL(num, a.read<unsigned int>(i).payload);
	}
}

