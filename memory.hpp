#pragma once

#define MAXMEMORY 524288000UL //The max memory of a pool.
#define PAGESIZE 1048576UL //1MB starting size. MAXMEMORY must be divisable by PAGESIZE
#define RANGECOUNT = MAXMEMORY/MemoryRangeSize //the number of memory ranges.
#define MEMOPOUTOFRANGE "MEMORY OUT OF RANGE OPERATION ERROR"

/**
 * returned by the memory read operations
*/
template<typename T>
struct readResult {
    T *payload;
    bool valid; //true if it was a valid read.
    bool needsFree=false;
};

/**
 * @brief represents a page of memory.
 * 
 */
class page {
private:
    unsigned long address;
    unsigned char * data;

    /**
     * @brief ensure a address is safe to read.
     * 
     * @param address the address
     * @return true if the address is valid and safe to read.
     * @return false if the address is not safe to read
     */
    bool safeToRead(unsigned long address);

    /**
     * @brief Get the Offset to read
     * 
     * @param address the address to read
     * @return unsigned long the offset in to the data to read.
     */
    unsigned long getOffset(unsigned long address);
public:

    /**
     * @brief Construct a new page object
     * 
     */
    page(unsigned long address);

    /**
     * @brief Destroy the page object
     * 
     */
    ~page();

    /**
     * @brief Read a byte.
     * 
     * @param address the address to read
     */
    readResult<unsigned char> readByte(unsigned long address);

    /**
     * @brief read data from self
     * 
     * @tparam  the Type T
     * @param address  the address to read
     * @return readResult<T>  stores the read result and some metadata.
     */
    template<typename T>
    readResult<T> read(unsigned long address);


    /**
     * @brief write a byte.
     * 
     * @param address the address to write
     * @param data the data to write
     */
    bool writeByte(unsigned long address, unsigned char data);

    /**
     * @brief write data from self
     * 
     * @tparam  the Type T
     * @param address  the address to write
     * @param data the data to write
     * @return readResult<T>  stores the write result and some metadata.
     */
    template<typename T>
    bool write(unsigned long address, T data);
};

class Memory {
private:
    page ** pages;


    /**
     * @brief make sure a specified address is valid and ensure a page is ready to be read at it.
     * 
     * @param pageId the id of the page
     * @return true if it is safe to procede with the read or write
     * @return false if it is not safe.
     */
    bool preparePage(unsigned long pageId);

    /**
     * @brief Get the Page object
     * @throws MEMOPOUTOFRANGE error if the memory page is invalid
     * @param address the address
     * @return page* the page for the address
     */
    page * getPage(unsigned long address);
public:
    Memory();
    ~Memory();

    /**
     * @brief Read a byte.
     * 
     * @param address the address to read
     */
    readResult<unsigned char> readByte(unsigned long address);

    /**
     * @brief read data from self
     * if needsFree is set it needs to be freed to prevent a memory leak.
     * @tparam  the Type T
     * @param address  the address to read
     * @return readResult<T>  stores the read result and some metadata.
     */
    template<typename T>
    readResult<T> read(unsigned long address);


    /**
     * @brief write a byte.
     * 
     * @param address the address to write
     * @param data the data to write
     */
    bool writeByte(unsigned long address, unsigned char data);

    /**
     * @brief write data from self
     * 
     * @tparam  the Type T
     * @param address  the address to write
     * @param data the data to write
     * @return readResult<T>  stores the write result and some metadata.
     */
    template<typename T>
    bool write(unsigned long address, T data);
};