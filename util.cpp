#include <util.hpp>
#include <vector>
#include <string>

findDelineatorResult findDelineator(std::string s, std::vector<std::string> delineator) {
    ulong first = std::string::npos;
    std::string usedDelineator;
    ulong now;
    std::vector<std::string>::iterator it = delineator.begin();
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

std::vector<std::string> splitString(std::string input, std::vector<std::string> delineator) {
    std::vector<std::string> result;
    findDelineatorResult pos;
    std::string token;
    while ((pos = findDelineator(input, delineator)).pos != std::string::npos) {
        token = input.substr(0, pos.pos);
        result.push_back(token);
        input.erase(0, pos.pos + pos.delineator.length());
    }
    return result;
}

std::vector<std::string> splitStringRemoveEmpty(std::string input, std::vector<std::string> delineator) {
    std::vector<std::string> result = splitString(input, delineator);
    std::vector<std::string>::iterator it = result.begin();
    while(it<result.end()) {
        if(it->empty()) {
            result.erase(it--);
        }
        it++;
    }
}