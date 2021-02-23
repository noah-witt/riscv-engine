#define BOOST_TEST_MODULE Simple testcases 1
//#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#include "../memory.hpp"
BOOST_AUTO_TEST_SUITE(int_write)

//BOOST_AUTO_TEST_CASE(simple_test) {
//	BOOST_CHECK_EQUAL(2+2, 4);
//}

BOOST_AUTO_TEST_CASE(simple_test) {
	//handle int writes and reads.
	for(int i =0; i< 100000; i++) {
		Memory a = Memory();
		int num = rand();
		unsigned long address = rand()%524288000UL;
		a.write<int>(address, num);
		BOOST_CHECK_EQUAL(num, a.read<int>(address));
	}
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(begin_to_end)


BOOST_AUTO_TEST_CASE(simple_test) {
	//handle int writes and reads.
	for(int i =0; i< (524288000UL/sizeof(int)); i++) {
		Memory a = Memory();
		int num = rand();
		unsigned long address = i*sizeof(int);
		a.write<int>(address, num);
		//BOOST_CHECK_EQUAL(num, a.read<int>(address));
		BOOST_CHECK_EQUAL(true, false);
	}
}

BOOST_AUTO_TEST_SUITE_END()