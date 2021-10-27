/**
 * @file start.cpp
 * @author Noah Witt <nawitt18@ole.augie.edu>
 * @brief tests for the start system
 * @version 0.1
 * @date 2021-10-18
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include "../start.hpp"

BOOST_AUTO_TEST_SUITE(start_test)
BOOST_AUTO_TEST_CASE(basic_start_test) {
    std::string a_input_data = "";
    std::stringstream a_input(a_input_data);
    std::stringstream a_output;
    start("./test_files/test_start_a.S", 10000, a_input, a_output);
    std::string result = a_output.str();
    BOOST_LOG_TRIVIAL(debug) <<"test:"<< result;
    BOOST_ASSERT(result=="105 775 ABC 3 976 3 ");
}

BOOST_AUTO_TEST_SUITE_END()
