#include "memory.hpp"
#include <stdlib.h>

unsigned long page::getOffset(unsigned long address) {
   return  address-this->address;
}

page::page(unsigned long address) {
    this->address=address;
    this->data = (unsigned char *)calloc(PAGESIZE,sizeof(unsigned char));
}

page::~page() {
    free(this->data);
}

readResult<unsigned char> page::readByte(unsigned long address) {
    readResult<unsigned char> result;
    if(!safeToRead(address)) {
        //not safe to read.
        result.valid = false;
        result.payload = nullptr;
        return result;
    }
    result.payload  = (this->data)+getOffset(address);
    result.valid = true;
    return result;
}

template<typename T>
readResult<T> page::read(unsigned long address) {
    readResult<T> result;
    if((!safeToRead(address))||(!safeToRead(address+sizeof(T)-1))) {
        //not safe to read.
        result.valid = false;
        result.payload = nullptr;
        return result;
    }
    result.payload  = (T *)((this->data)+getOffset(address));
    result.valid = true;
    return result;
}

bool page::writeByte(unsigned long address, unsigned char data) {
    if(!safeToRead(address)) {
        //not safe to write.
        return false;
    }
    (this->data)[getOffset(address)] = data;
    return true;
}

template<typename T>
bool page::write(unsigned long address, T data) {
    if((!safeToRead(address))||(!safeToRead(address+sizeof(T)-1))) {
        //not safe to write.
        return false;
    }
    *((T *)((this->data)+getOffset(address))) = data; //write to the location specified by the data pointer offset.
    return true;
}