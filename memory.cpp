#include <stdlib.h> 
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
        bool expand() {
            if(this->fixedSize) return false; //do not expand if this has a specific size.
            if(this->size>=this->maxSize) return false; //do not expand beyond the limits.
            unsigned long newSize = this->size+this->sizeIncr;
            if(newSize>this->maxSize) newSize = this->maxSize;
            //new size is now the ideal store size
            this->store =(unsigned char *) realloc(this->store, newSize); //reallocate to a larger block.
            this->size = newSize;
            return true;
        }
        /**
         * returns the offset in to the store from the external location
         */
        unsigned long getOffset(unsigned long external) {
            long offset = external-this->start;
            if(offset<0) offset=-1*offset;
            return offset;
        }
        /**
         * Calculate and set the maximum upper and lower level for this allocation
         */
        void calculateMaxUpperAndLower() {
            unsigned long a = this->start;
            unsigned long b = this->start+(((this->growDown)?1:-1)*this->maxSize);
            this->maxLower  = (a<b)?a:b;
            this->maxUpper  = (a<b)?b:a;
        }

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
         */
        MemoryRange(unsigned long start, bool growDown, unsigned long maxSize) {
            this->start  = start;
            this->growDown=growDown;
            this->store=(unsigned char *)malloc(this->initSize);
            this->size=this->initSize;
            this->fixedSize=false;
            this->maxSize = maxSize;
            calculateMaxUpperAndLower();
        }
        /**
         * Create an allocation of a fixed specific size.
         */
        MemoryRange(unsigned long start, unsigned long end) {
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
        ~MemoryRange(){
            //free up this space
            free(this->store);
        }
        /**
         * returns true if this memoryRange contains this location
         * @returns true if this is in the memory range, false otherwise.
         */
        bool contains(unsigned long location) {
            return location>=maxLower&&location<=maxUpper;
        }

        //WRITE methods

        /**
         * write a single byte
         * @returns true if the write worked.
         * @param location is the memory location to write to
         * @param value is the value to be stored in memory.
         */
        bool writeByte(unsigned long location,unsigned char value) {
            if(!(contains(location))) throw "segfault"; //attempted to write to a location that is not allowed.
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
         * write a four bytes
         * @returns true if the write worked.
         * @param location is the memory location to write to
         * @param value is the value to be stored in memory.
         */
        bool write4byte(unsigned long location,u_int32_t value) {
            if(!(contains(location)&&contains(start+3))) throw "segfault"; //attempted to write to a location that is not allowed.
            unsigned long offset= getOffset(location);
            while(!safeToRead(offset)||!safeToRead(offset+3)) {
                //we must expand the memory space until the address is writeable.
                if(!expand()) throw "out of memory"; //this case should never be hit. it should be managed by the memory class.
                //expanded store
            }
            //it is safe to write 
           ((u_int32_t *) store)[offset/4] = value;
            return true;
        }
        
        /**
         * write a eight bytes
         * @returns true if the write worked.
         * @param location is the memory location to write to
         * @param value is the value to be stored in memory.
         */
        bool write4byte(unsigned long location,u_int64_t value) {
            if(!(contains(location)&&contains(start+7))) throw "segfault"; //attempted to write to a location that is not allowed.
            unsigned long offset= getOffset(location);
            while(!safeToRead(offset)||!safeToRead(offset+7)) {
                //we must expand the memory space until the address is writeable.
                if(!expand()) throw "out of memory"; //this case should never be hit. it should be managed by the memory class.
                //expanded store
            }
            //it is safe to write 
           ((u_int64_t *) store)[offset/8] = value;
            return true;
        }


        //READ methods.

        /**
         * read a single byte, return zero if it is not allocated yet.
         * @returns the value at the specified memory location
         * @param start is the memory location to be read.
         */
        unsigned char readByte(unsigned long start) {
            if(!(contains(start))) throw "segfault"; //do not read the data if it is a bad read.
            unsigned long startByte= getOffset(start);
            if(safeToRead(startByte)) return store[startByte];
            return 0;
        }

        /**
         * read in a 32 bit long value
         * @returns the specified value
         * @param start the memory location to read from
         */
        u_int32_t read4byte(unsigned long start) {
            if(!(contains(start)&&contains(start+3))) throw "segfault"; //do not read the data if it is a bad read.
            //is the byte to look at in the store.
            unsigned long startByte= getOffset(start);
            if(!safeToRead(startByte)) return 0; //handle the case where the whole thing is out of memory, so just dump a 0.
            while(!safeToRead(startByte)||!safeToRead(startByte+3)){
                //we must expand the address space.
                //this case should not be touched because we should never read back uninitalized address space... 
                if(!expand()) throw "partial read of uninitialized memory space. unable to expand memory space. unmanageable."; //can not grow, just segfault and go on.
                //expanded successfully.
            }
            //memory location validated as safe to read
            u_int32_t temp = ((u_int32_t *) store)[startByte/4]; 
        }

        /**
         * read in a 64 bit long value
         *  @returns the specified value
         *  @param start the memory location to read from
         */
        u_int64_t read8byte(unsigned long start) {
            if(!(contains(start)&&contains(start+7))) throw "segfault"; //do not read the data if it is a bad read.
            //is the byte to look at in the store.
            unsigned long startByte= getOffset(start);
            if(!safeToRead(startByte)) return 0; //handle the case where the whole thing is out of memory, so just dump a 0.
            while(!safeToRead(startByte)||!safeToRead(startByte+7)){
                //we must expand the address space.
                //this case should not be touched because we should never read back uninitalized address space... 
                if(!expand()) throw "partial read of uninitialized memory space. unable to expand memory space. unmanageable."; //can not grow, just segfault and go on.
                //expanded successfully.
            }
            //memory location validated as safe to read
            u_int64_t temp = ((u_int64_t *) store)[startByte/8]; 
        }
        
};

/**
 * Represents the memory mapped area
 */
class MemoryMapArea {
    //TODO init
    //TODO destruct
    //TODO create
    //TODO read/write
};

class Memory {
    private:
        const unsigned long maxMemory = 524288000; // 500MB of memory 500*1024^2. long because memory locations can be beyond 4096 bytes.
        MemoryRange low;
        MemoryRange upper;
    public:
        //TODO init
        //TODO destruct
        //TODO read
        //TODO manage reads across segments
        //TODO write
        //TODO manage writes across segments

};
