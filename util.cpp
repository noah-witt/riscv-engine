#include "./util.hpp"
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <iostream>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/expressions.hpp>

findDelineatorResult findDelineator(std::string const &s, std::vector<std::string> const &delineator) {
    ulong first = std::string::npos;
    std::string usedDelineator;
    ulong now;
    std::vector<std::string>::const_iterator it = delineator.begin();
    while(it<delineator.end()) {
        now = s.find(*it);
        if (now<first) {
            first = now;
            usedDelineator = *it;
        }
        it++;
    }
    findDelineatorResult result;
    result.pos = first;
    result.delineator = usedDelineator;
    return result;
}

std::vector<std::string> splitString(std::string const &input, std::vector<std::string> const &delineator) {
    std::string inputLocal = input+"";
    std::vector<std::string> result;
    findDelineatorResult pos;
    std::string token;
    while ((pos = findDelineator(inputLocal, delineator)).pos != std::string::npos) {
        token = inputLocal.substr(0, pos.pos);
        result.push_back(token);
        inputLocal.erase(0, pos.pos + pos.delineator.length());
    }
    // add the end
    result.push_back(inputLocal);
    return result;
}

std::vector<std::string> splitStringRemoveEmpty(std::string const &input, std::vector<std::string> const &delineator) {
    std::vector<std::string> result = splitString(input, delineator);
    std::vector<std::string>::iterator it = result.begin();
    while(it<result.end()) {
        if(it->empty()) {
            result.erase(it--);
        }
        it++;
    }
    return result;
}

bool strEndsIn(std::string const &str, std::string const &end) {
    if (str.length() >= end.length()) {
        return (0 == str.compare(str.length() - end.length(), end.length(), end));
    }
    return false;
}

// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}

bool isNumber(const std::string& s)
{
    for(std::string::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        if(*it=='-') continue;
        if(!std::isdigit(*it)) return false;
    }
    return true;
}
