/**
 * @file assemble.hpp
 * @author Noah Witt (nawitt18@ole.augie.edu)
 * @version 0.1
 * @date 2021-09-01
 * 
 */

#include <string>
#include <unordered_map>
#include "registers.hpp"

/**
 * @brief represents a symbol
 * 
 */
class Symbol {
    protected:
        std::string symbol;
        unsigned long referenceLocation;
        unsigned int size;
        /**
         * Fixed is true if the memory location does not change, it is fixed
         */
        bool fixed;
    public:
        Symbol(std::string symbol, unsigned int size);
        Symbol(std::string symbol, unsigned long address, unsigned int size);
        ~Symbol();
        unsigned long getAddress();
        unsigned int getSize();
        bool getFixed();
        std::string getSymbol();
        bool matchesSymbol(std::string symbol);
};

class SymbolTable {
    protected:
        std::unordered_map<std::string, Symbol> table;
    public:
        /**
         * @brief insert a symbol with a assigned location
         * 
         * @param symbol the name
         * @param size the size
         * @return true it worked
         * @return false it failed
         */
        bool insert(std::string symbol, unsigned int size);
        /**
         * @brief insert a symbol at a fixed location
         * 
         * @param symbol the name
         * @param address the address of the location
         * @param size the size of the thing
         * @return true if it worked
         * @return false if it worked
         */
        bool insert(std::string symbol, unsigned long address, unsigned int size);
        /**
         * @brief Remove a specified symbol.
         * 
         * @param symbol the symbol name to remove
         * @return Symbol the removed symbol.
         */
        Symbol remove(std::string symbol);
        /**
         * @brief Find a symbol
         * 
         * @param symbol the name of the symbol
         * @return Symbol the object 
         */
        Symbol find(std::string symbol);
        /**
         * @brief Layout the memory of this symbol table
         * 
         * @param startAddress the address to start the layout from
         * @return true it worked
         * @return false it failed
         */
        bool layout(unsigned long startAddress);
};

class Instruction {
    protected:
        std::string value;
    public:
        /**
         * @brief Construct a new Instruction object
         * 
         * @param value the instruction string line
         */
        Instruction(std::string value);
        /**
         * @brief a register with the value
         * @note Free the register after your done with it.
         * @returns Register with the value
         */
        Register getInstruction();
};