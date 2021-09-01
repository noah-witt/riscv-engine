/**
 * @file compile.hpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @version 0.1
 * @date 2021-09-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string>


/**
 * @brief represents a symbol
 * 
 */
class Symbol {
    protected:
        std::string symbol;
        unsigned long referenceLocation;
    public:
        Symbol();
        ~Symbol();
        unsigned long getAddress();
        std::string getSymbol();
        bool matchesSymbol(std::string symbol);
};

class SymbolTable {
    protected:
        // TODO some sort of list structure
    public:
        //TODO insert add etc
};