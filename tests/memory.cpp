#define BOOST_TEST_MODULE memory_test
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#include "../memory.hpp"

/*
BOOST_AUTO_TEST_CASE(write_random_int_test) {
	BOOST_CHECK_EQUAL(true, true);
	BOOST_CHECK_EQUAL(true, false);
}*/



BOOST_AUTO_TEST_CASE(write_random_int_test) {
	//handle int writes and reads.
	for(int i =0; i< 100000; i++) {
		Memory a = Memory();
		int num = rand();
		unsigned long address = rand()%524288000UL;
		a.writeByte(address,num);
		//a.write<int>(address, num);
		//BOOST_CHECK_EQUAL(num, a.read<int>(address));
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

