#include <stdlib.h> 
#include <stdio.h>
#include <vector>
#include "memory.hpp"



bool MemoryRange::expand() {
    if(this->fixedSize) return false; //do not expand if this has a specific size.
    if(this->size>=this->maxSize) return false; //do not expand beyond the limits.
    unsigned long newSize = this->size+this->sizeIncr;
    if(newSize>this->maxSize) newSize = this->maxSize;
    //new size is now the ideal store size
    this->store =(unsigned char *) realloc(this->store, newSize); //reallocate to a larger block.
    this->size = newSize;
    return true;
}

unsigned long MemoryRange::getOffset(unsigned long external) {
    long offset = external-this->start;
    if(offset<0) offset=-1*offset;
    return offset;
}

void MemoryRange::calculateMaxUpperAndLower() {
    unsigned long a = this->start;
    unsigned long b = this->start+(((this->growDown)?1:-1)*this->maxSize);
    this->maxLower  = (a<b)?a:b;
    this->maxUpper  = (a<b)?b:a;
}

MemoryRange::MemoryRange(unsigned long start, bool growDown, unsigned long maxSize) {
    this->start  = start;
    this->growDown=growDown;
    this->store=(unsigned char *)malloc(this->initSize);
    this->size=this->initSize;
    this->fixedSize=false;
    this->maxSize = maxSize;
    calculateMaxUpperAndLower();
}

MemoryRange::MemoryRange(unsigned long start, unsigned long end) {
    unsigned long lower = (start<end)?start:end;
    unsigned long upper = (start<end)?end:start;
    this->start=lower;
    this->size=upper-lower;
    this->fixedSize=true;
    this->growDown=false;
    this->maxSize=upper-lower;
    this->store=(unsigned char *)malloc(upper-lower);
    calculateMaxUpperAndLower();
}

/**
* Deconstruct the allocation
*/
MemoryRange::~MemoryRange(){
    //free up this space
    free(this->store);
}

/**
 * returns true if this memoryRange contains this location
 * @returns true if this is in the memory range, false otherwise.
 */
bool MemoryRange::contains(unsigned long location) {
    return location>=maxLower&&location<=maxUpper;
}

/**
 * write a single byte
 * @returns true if the write worked.
 * @param location is the memory location to write to
 * @param value is the value to be stored in memory.
 */
bool MemoryRange::writeByte(unsigned long location,unsigned char value) {
    printf("loc:\t%p;\nmaxL:\t%p; \nmaxU:\t%p;\n", location, maxLower, maxUpper);
    if(!(contains(location))) throw "segfault "; //attempted to write to a location that is not allowed.
    unsigned long offset= getOffset(location);
    while(!safeToRead(offset)) {
        //we must expand the memory space until the address is writeable.
        if(!expand()) throw "out of memory"; //this case should never be hit. it should be managed by the memory class.
        //expanded store
    }
    //it is safe to write 
    store[offset]= value; //write the byte.
    return true;
}

/**
 * write the specified type
 * @returns true if the write worked.
 * @param location is the memory location to write to
 * @param value is the value to be stored in memory.
 */
template<typename T>
bool MemoryRange::write(unsigned long location,T value) {
    unsigned int typeSize = sizeof(T);
    if(!(contains(location)&&contains(start+typeSize-1))) throw "segfault"; //attempted to write to a location that is not allowed.
    unsigned long offset= getOffset(location);
    while(!safeToRead(offset)||!safeToRead(offset+typeSize-1)) {
        //we must expand the memory space until the address is writeable.
        if(!expand()) throw "out of memory"; //this case should never be hit. it should be managed by the memory class.
        //expanded store
    }
    //it is safe to write 
    ((T)(store[offset])) = value;
    return true;
}

/**
 * read a single byte, return zero if it is not allocated yet.
 * @returns the value at the specified memory location
 * @param start is the memory location to be read.
 * @param T the type to be read.
 */
unsigned char MemoryRange::readByte(unsigned long start) {
    if(!(contains(start))) throw "segfault"; //do not read the data if it is a bad read.
    unsigned long startByte= getOffset(start);
    if(safeToRead(startByte)) return store[startByte];
    return 0;
}

/**
 * @returns the specified type
 * @param start the memory location to read from
 * @param T the type to be read.
 */
template<typename T>
T MemoryRange::read(unsigned long start) {
    unsigned int typeSize = sizeof(T);
    if(!(contains(start)&&contains(start+typeSize+1))) throw "segfault"; //do not read the data if it is a bad read.
    //is the byte to look at in the store.
    unsigned long startByte= getOffset(start);
    if(!safeToRead(startByte)) return 0; //handle the case where the whole thing is out of memory, so just dump a 0.
    while(!safeToRead(startByte)||!safeToRead(startByte+typeSize-1)){
        //we must expand the address space.
        //this case should not be touched because we should never read back uninitalized address space... 
        if(!expand()) throw "partial read of uninitialized memory space. unable to expand memory space. unmanageable."; //can not grow, just segfault and go on.
        //expanded successfully.
    }
    //memory location validated as safe to read
    T* temp = (T *)(&(store[startByte]));
    return temp;
}


/**
 * Validates if it contains a specified value.
 * @returns true if it contains the specified locations and it is safe to procede.
 */
bool Memory::contains(unsigned long address, unsigned char size) {
    return address+size-1<=this->size;
}

/**
 * @returns a pointer to the memory range that should be read or null if it can not be read.
 */
MemoryRange * Memory::getRange(unsigned long address) {
    printf("getRange\na:\t%p\nlower:\t%p;\nSize:\t%p;\n", address, low, size);
    if(address<=lowerMax) return low;
    if(address<=size) return upper;
    //TODO memory mapped region.
    return nullptr;
}

/**
 * Create the memory system with bounds that grow aproproately
 */
Memory::Memory () {
    unsigned long lowSize = (MAXMEMORY)/2UL;
    lowerMax = lowSize;
    unsigned long upperSize = MAXMEMORY-lowSize;
    low = new MemoryRange(0, false, lowSize);
    upper = new MemoryRange(MAXMEMORY, true, upperSize);
    io = new std::vector<MemoryMapArea>();
    size = MAXMEMORY;
}

/**
 * Free my pointers
 */
Memory::~Memory() {
    free(low);
    free(upper);
    free(io);
}

/**
 * read a single byte
 * @returns a reedResult struct which contains the data or 0 if it is out of range and sets a flag.
 * @param address the address to the memory
 */
readResult<unsigned char> Memory::readByte(unsigned long address) {
    //TODO handle reads from memory mapped io.
    readResult<unsigned char> result;
    if(address<=lowerMax) {
        result.payload = low->readByte(address);
        result.valid = true;
        return result;
    }
    if(address<=size) {
        result.payload = upper->readByte(address);
        result.valid = true;
        return result;
    }
    //return zero for spurious reads.
    result.payload = 0;
    result.valid = false;
    return result;
}

/**
 * Read
 * @returns a readResult struct with the data
 * handles reads across regions.
 * @param address the address to the memory
 * @param T the type to be read.
 */
template<typename T>
readResult<T> Memory::read(unsigned long address) {
    unsigned int typeSize = sizeof(T);
    MemoryRange *startRegion = getRange(address);
    MemoryRange *endRegion = getRange(address+typeSize-1);
    if(startRegion==endRegion) {
        readResult<T> result;
        result.payload = startRegion->read<T>(address);
        result.valid = true;
        return result;
    }

    //handle when there is a split
    T * workSpace = malloc(typeSize);   //workspace is where we will construct the area byte by byte.
    bool valid = true;  //is set to false if any of the byte reads are invalid.
    //construct it byte by byte.
    for(unsigned int i=0;i<typeSize; i++) {
        readResult<T> temp = readByte(address); //the byte we are adding to the loop
        ((unsigned char *)workSpace)[i] = temp.payload;
        if(!temp.valid) valid=false;
    }
    readResult<T> result;
    result.payload = *workSpace;
    result.valid = valid;
    free(workSpace);    //IMPORTANT! used malloc here must be freed.
    return result;
}

/**
 * write a single byte
 * @returns true if it worked.
 * @param address the address to the memory
 * @param value the value to write.
 */
bool Memory::writeByte(unsigned long address, unsigned char value) {
    //TODO handle writes from memory mapped io.

    MemoryRange *region = getRange(address);
    if(region==nullptr) return false;
    return region->writeByte(address, value);
}

/**
 * write
 * @returns true if it worked.
 * handles writes across regions.
 * @param address the address to the memory
 * @param T the type to be written.
 * @param value the value to write.
 */
template<typename T>
bool Memory::write(unsigned long address, T value) {
    unsigned int typeSize = sizeof(T);
    MemoryRange *startRegion = getRange(address);
    MemoryRange *endRegion = getRange(address+typeSize-1);
    if(startRegion==endRegion) {
        return startRegion->write<T>(address, value);
    }

    //handle when there is a split
    bool valid = true;  //is set to false if any of the byte writes are invalid.
    //construct it byte by byte.
    for(unsigned int i=0;i<typeSize; i++) {
        bool temp = writeByte(address, value);
        if(!temp) valid=false;
    }
    return valid;
}

