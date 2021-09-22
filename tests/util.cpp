#define BOOST_TEST_MODULE util_test
#include <boost/test/included/unit_test.hpp>
#include "../util.hpp"
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>


BOOST_AUTO_TEST_CASE(find_delinator_test) {
    std::vector<std::string> del;
    del.push_back(";");
    findDelineatorResult res = findDelineator("a;baa; ads;", del);
    BOOST_ASSERT(res.pos == 1);
    BOOST_ASSERT(res.delineator == ";");
    res = findDelineator("c ;d dd", del);
    BOOST_ASSERT(res.pos == 2);
}

BOOST_AUTO_TEST_CASE(split_string_basic_test) {
    std::vector<std::string> delinator;
    delinator.push_back(" ");
    delinator.push_back(",");
    std::vector<std::string> result = splitString("a123 b123, c123 d123,e123,f123", delinator);
    BOOST_ASSERT(result.size() == 7);
    BOOST_ASSERT(result.at(0) == "a123");
    BOOST_ASSERT(result.at(1) == "b123");
    BOOST_ASSERT(result.at(2) == "");
    BOOST_ASSERT(result.at(3) == "c123");
    BOOST_ASSERT(result.at(4) == "d123");
    BOOST_ASSERT(result.at(5) == "e123");
    BOOST_ASSERT(result.at(6) == "f123");
}

BOOST_AUTO_TEST_CASE(split_string_remove_empty_test) {
    std::vector<std::string> delinator;
    delinator.push_back(" ");
    delinator.push_back(",");
    std::vector<std::string> result = splitStringRemoveEmpty("a123 b123, c123 d123,e123,f123", delinator);
    BOOST_ASSERT(result.size() == 6);
    BOOST_ASSERT(result.at(0) == "a123");
    BOOST_ASSERT(result.at(1) == "b123");
    BOOST_ASSERT(result.at(2) == "c123");
    BOOST_ASSERT(result.at(3) == "d123");
    BOOST_ASSERT(result.at(4) == "e123");
    BOOST_ASSERT(result.at(5) == "f123");
}

BOOST_AUTO_TEST_CASE(isNumber_test) {
    BOOST_ASSERT(isNumber("-123") == true);
    BOOST_ASSERT(isNumber("123") == true);
    BOOST_ASSERT(isNumber("s-s") == false);
    BOOST_ASSERT(isNumber("asd") == false);
}
