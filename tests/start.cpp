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

#define BOOST_TEST_MODULE start_test
#include <boost/test/included/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include "../start.hpp"

BOOST_AUTO_TEST_CASE(basic_start_test) {
    std::string a_input_data = "";
    std::stringstream a_input(a_input_data);
    std::stringstream a_output;
    start("./tests/test_start_a.S", 10000, a_input, a_output);
    std::string result = a_output.str();
    BOOST_LOG_TRIVIAL(debug) <<"test:"<< result;
    BOOST_ASSERT(result=="105 775 ABC");
    BOOST_ASSERT(false==true); // a temp expression to force this to fail at the end.
}
