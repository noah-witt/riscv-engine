#define BOOST_TEST_MODULE memory_test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#include "../memory.hpp"
#include "../memory.t.hpp"


BOOST_AUTO_TEST_CASE(first_write_test) {
	Memory a = Memory();
	a.writeByte(0,0xa);
	BOOST_CHECK_EQUAL(0xa, *(a.readByte(0).payload));
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
		BOOST_CHECK_EQUAL(num, *(a.readByte(address).payload));
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
		BOOST_CHECK_EQUAL(num, *(a.read<unsigned int>(address).payload));
	}
}

/*
BOOST_AUTO_TEST_CASE(write_full_int_test) {
	//handle int writes and reads.
	for(int i =0; i< (524288000UL/sizeof(int)); i++) {
		Memory a = Memory();
		int num = rand();
		unsigned long address = i*sizeof(int);
		a.write<int>(address, num);
		//BOOST_CHECK_EQUAL(num, a.read<int>(address));
		BOOST_CHECK_EQUAL(true, false);
	}
}*/

