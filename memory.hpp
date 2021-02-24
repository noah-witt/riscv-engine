#pragma once

#define MAXMEMORY 524288000UL 
/**
 * returned by the memory read operations
*/
template<typename T>
struct readResult {
    T payload;
    bool valid; //true if it was a valid read.
};

/**
 * Stores and manages a memory range
 * handles writes and reads
 * is used by higher level objects to actually implement storage
 */
class MemoryRange {
    private:
        const unsigned long initSize = 1048576; //1MB starting size.
        const unsigned long sizeIncr = 10485760; //10MB, increase by 10 MB for every subsequent increase in size.
        unsigned char * store;
        unsigned long size;
        unsigned long start;
        unsigned long maxSize;
        bool growDown;
        bool fixedSize;
        unsigned long maxLower;
        unsigned long maxUpper;

        /**
         * expand expands the memory to the next step of size.
         * @returns true if it successfully expanded, false otherwise.
         */
        bool expand();

        /**
         * returns the offset in to the store from the external location
         * @param external is the addres
         * @returns the offset in to this store.
         */
        unsigned long getOffset(unsigned long external);

        /**
         * Calculate and set the maximum upper and lower level for this allocation
         */
        void calculateMaxUpperAndLower();

        /**
         * determines if it is safe to read a specified byte
         * @returns true if it is safe, false otherwise
         */
        bool safeToRead(unsigned long offset) {
            return offset<size;
        }

    public:
        /**
         * Construct memory location 
         * @param start is the address that this allocation starts at
         * @param growDown controls the direction that the allocation expands toward. true is down
         * @param maxsize is the maximum numbeer of bytes this object can grow to store.
         */
        MemoryRange(unsigned long start, bool growDown, unsigned long maxSize);

        /**
         * Create an allocation of a fixed specific size.
         * this range then will initialize the whole allocation at the start.
         * @param start is the starting byte location
         * @param end is the ending byte location
         */
        MemoryRange(unsigned long start, unsigned long end);

        /**
         * cleanup
         */
        ~MemoryRange();

        /**
         * returns true if this memoryRange contains this location
         * @returns true if this is in the memory range, false otherwise.
         */
        bool contains(unsigned long location);

        //WRITE methods

        /**
         * write a single byte
         * @returns true if the write worked.
         * @param location is the memory location to write to
         * @param value is the value to be stored in memory.
         */
        bool writeByte(unsigned long location,unsigned char value);

        /**
         * write the specified type
         * @returns true if the write worked.
         * @param location is the memory location to write to
         * @param value is the value to be stored in memory.
         */
        template<typename T>
        bool write(unsigned long location,T value);

        //READ methods.

        /**
         * read a single byte, return zero if it is not allocated yet.
         * @returns the value at the specified memory location
         * @param start is the memory location to be read.
         * @param T the type to be read.
         */
        unsigned char readByte(unsigned long start);

        /**
         * @returns the specified type
         * @param start the memory location to read from
         * @param T the type to be read.
         */
        template<typename T>
        T read(unsigned long start);
        
};

/**
 * Represents the memory mapped area
 * these virtual devices register memory mapped i/o with the Memory subsystem. 
 * virtual devices get to interact with the memory subsystem.
 * 
 * memory mapped ranges must be of a static size, so most devices should just accept a pointer to the location where the actual data is.
 * for example a display should take a pointer to a string that it will display.
 */
class MemoryMapArea: public MemoryRange {
    //TODO construct
    //TODO destruct
    //TODO create
    //TODO read/write
};


/**
 * Has two banks of memory, the upper and lower range. The stack area is the upper area and grows down. The heap area is the lower area and grows up
 * this allows large areas of memory to be allocated, but not initalized on the underlying system.
 * 
 * Memory mapped i/o is bound to some high value beyond the allocation.
 * Memory mapped i/o contains some data area that the device reads. 
 */
class Memory {
     private:
        unsigned long size;
        MemoryRange * low;
        unsigned long lowerMax;
        MemoryRange * upper;
        std::vector<MemoryMapArea> * io;

        /**
         * Validates if it contains a specified value.
         * @returns true if it contains the specified locations and it is safe to procede.
         */
        bool contains(unsigned long address, unsigned char size);

        /**
         * @returns a pointer to the memory range that should be read or null if it can not be read.
         */
        MemoryRange * getRange(unsigned long address);
    public:

    /**
     * Create the memory system with bounds that grow aproproately
     */
    Memory ();

    /**
     * cleanup
     */
    ~Memory();

    /**
    * read a single byte
    * @returns a reedResult struct which contains the data or 0 if it is out of range and sets a flag.
    * @param address the address to the memory
    * 
    */
    readResult<unsigned char> readByte(unsigned long address);

    /**
     * Read
     * @returns a readResult struct with the data
     * handles reads across regions.
     * @param address the address to the memory
     * @param T the type to be read.
     */
    template<typename T>
    readResult<T> read(unsigned long address);

    /**
     * write a single byte
     * @returns true if it worked.
     * @param address the address to the memory
     * @param value the value to write.
     */
    bool writeByte(unsigned long address, unsigned char value);

    /**
     * write
     * @returns true if it worked.
     * handles writes across regions.
     * @param address the address to the memory
     * @param T the type to be written.
     * @param value the value to write.
     */
    template<typename T>
    bool write(unsigned long address, T value);
};