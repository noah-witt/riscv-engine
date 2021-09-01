/**
 * @file assemble.hpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @version 0.1
 * @date 2021-09-01
 * 
 */

#include <string>
#include <unordered_map>


/**
 * @brief represents a symbol
 * 
 */
class Symbol {
    protected:
        std::string symbol;
        unsigned long referenceLocation;
        unsigned int size;
    public:
        Symbol(std::string symbol, unsigned long address, unsigned int size);
        ~Symbol();
        unsigned long getAddress();
        unsigned int getSize();
        std::string getSymbol();
        bool matchesSymbol(std::string symbol);
};

class SymbolTable {
    protected:
        std::unordered_map<std::string, Symbol> table;
    public:
        bool insert(std::string symbol, unsigned int address);
        Symbol remove(std::string symbol);
        Symbol find(std::string symbol);
        //TODO get address 
};