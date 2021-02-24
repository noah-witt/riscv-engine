#pragma once
/**
 * returned by the memory read operations
*/
template<typename T>
struct readResult {
    T payload;
    bool valid; //true if it was a valid read.
};

/**
 * Has two banks of memory, the upper and lower range. The stack area is the upper area and grows down. The heap area is the lower area and grows up
 * this allows large areas of memory to be allocated, but not initalized on the underlying system.
 * 
 * Memory mapped i/o is bound to some high value beyond the allocation.
 * Memory mapped i/o contains some data area that the device reads. 
 */
class Memory {
    public:

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