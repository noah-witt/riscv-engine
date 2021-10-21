#pragma once
#include "string"
#include "unordered_map"


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
        Symbol();
        Symbol(std::string symbol, unsigned long address, unsigned int size);
        ~Symbol();
        unsigned long getAddress();
        unsigned int getSize();
        std::string getSymbol();
        bool matchesSymbol(std::string symbol);
        void setRef(unsigned long address);
};

struct SymbolTableFindResult {
    bool found;
    Symbol * symbol = nullptr;
};

class SymbolTable {
    protected:
        std::unordered_map<std::string, Symbol> table;
    public:
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
         * @param symbol the name of the symbol or a NULL
         * @return Symbol the object 
         */
        SymbolTableFindResult find(std::string symbol);
        /**
         * @brief Layout the memory of this symbol table
         * 
         * @param startAddress the address to start the layout from
         * @return true it worked
         * @return false it failed
         */
        bool layout(unsigned long startAddress);
        std::unordered_map<std::string, Symbol>::iterator begin();
        std::unordered_map<std::string, Symbol>::iterator end();
};