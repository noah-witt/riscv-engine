#include <stdlib.h> 
class MemoryRange {
    private:
        const unsigned long initSize = 1048576; //1MB starting size.
        const unsigned long sizeIncr = 10485760; //10MB, increase by 10 MB for every subsequent increase in size.
        char * store;
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
            this->store =(char *) realloc(this->store, newSize); //reallocate to a larger block.
            this->size = newSize;
            return true;
        }
        /**
         * returns the offset in to the store from the external location
         */
        long getOffset(unsigned long external) {
            long offset = external-this->start;
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
    public:
        /**
         * Construct memory location 
         */
        MemoryRange(unsigned long start, bool growDown, unsigned long maxSize) {
            this->start  = start;
            this->growDown=growDown;
            this->store=(char *)malloc(this->initSize);
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
            this->store=(char *)malloc(upper-lower);
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
        //TODO: Read
        //TODO: ensure reads only read allocated space or return zero.
        //TODO: prevent overruns.
        
};
class Memory {
    private:
        const unsigned long maxMemory = 524288000; // 500MB of memory 500*1024^2. long because memory locations can be beyond 4096 bytes.

};