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
findDelineatorResult findDelineator(std::string s, std::vector<std::string> delineator);

/**
 * @brief split a string based on the given delinators
 * 
 * @param input the string
 * @param delineator the list of delinators
 * @return std::vector<std::string> the split string.
 */
std::vector<std::string> splitString(std::string input, std::vector<std::string> delineator);

/**
 * @brief similar to splitString but also removes the empty strings
 * 
 * @param input the string
 * @param delineator the list of delinators
 * @return std::vector<std::string> the split string.
 */
std::vector<std::string> splitStringRemoveEmpty(std::string input, std::vector<std::string> delineator);
