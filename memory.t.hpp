#pragma once

#include "./memory.hpp"

template<typename T>
readResult<T> page::read(unsigned long address) {
    readResult<T> result;
    if((!safeToRead(address))||(!safeToRead(address+sizeof(T)-1))) {
        //not safe to read.
        result.valid = false;
        result.payload = 0x0;
        return result;
    }
    result.payload  = *((T *)((this->data)+getOffset(address)));
    result.valid = true;
    return result;
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

template<typename T>
readResult<T> Memory::read(unsigned long address) {
    try {

        page * p = getPage(address);
        page * p2 = getPage(address+sizeof(T)-1);
        if(p!=p2) {
            //handle when it is reading across pages.
            unsigned char target[sizeof(T)]; 
            bool valid = true;
            for(int i=0; i<sizeof(T); i++) {
                page * at = getPage(address+i);
                readResult<unsigned char> temp = at->readByte(address+i);
                target[i] = temp.payload;
                if(!temp.valid) valid = false;
            }
            readResult<T> result;
            result.valid = valid;
            result.payload = *((T *)(target));
            return result;
        }
        return p->read<T>(address);
    } catch(int e) {
        readResult<T> result;
        result.valid = false;
        result.payload = 0x0;
        return result;
    }
    //should not be possible.
}

template<typename T>
bool Memory::write(unsigned long address, T data) {
    try {

        page * p = getPage(address);
        page * p2 = getPage(address+sizeof(T)-1);
        unsigned char * list = (unsigned char *) (&data);
        if(p!=p2) {
            //handle when it is writing across pages.
            bool valid = true;
            for(int i=0; i<sizeof(T); i++) {
                page * at = getPage(address+i);
                bool temp = at->writeByte(address+i, list[i]);
                if(!temp) valid = false;
            }
            return valid;
        }
        return p->write<T>(address, data);
    } catch(int e) {
        return false;
    }
    //should not be possible.
}