#pragma once

#include <string>
#include <vector>

struct findDelineatorResult {
    ulong pos;
    std::string delineator; 
};

/**
 * @brief find the first occurrence of one of the delinators in the vector.
 * 
 * @param s the string
 * @param delineator the delinators
 * @return findDelineatorResult the result
 */
findDelineatorResult findDelineator(std::string const &s, std::vector<std::string> const &delineator);

/**
 * @brief split a string based on the given delinators
 * 
 * @param input the string
 * @param delineator the list of delinators
 * @return std::vector<std::string> the split string.
 */
std::vector<std::string> splitString(std::string const &input, std::vector<std::string> const &delineator);

/**
 * @brief similar to splitString but also removes the empty strings
 * 
 * @param input the string
 * @param delineator the list of delinators
 * @return std::vector<std::string> the split string.
 */
std::vector<std::string> splitStringRemoveEmpty(std::string const &input, std::vector<std::string> const &delineator);


bool strEndsIn(std::string const &str, std::string const &end);

std::string &ltrim(std::string &s);
std::string &rtrim(std::string &s);
std::string &trim(std::string &s);

/**
 * @brief validates if a string is only comprised of a numbers and -.
 * 
 * @param s the string.
 * @return true it is a number.
 * @return false it is not a number.
 */
bool isNumber(const std::string& s);
