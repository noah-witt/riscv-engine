#pragma once

#include "./memory.hpp"
#include "./alu.hpp"
#include "string"
#include <boost/log/trivial.hpp>

template<typename T>
readResult<T> page::read(unsigned long address) {
    readResult<T> result;
    if((!this->safeToRead(address))||(!this->safeToRead(address+sizeof(T)-1))) {
        //not safe to read.
        BOOST_LOG_TRIVIAL(error) << "read is beyond this pages bounds "<<address;
        result.valid = false;
        result.payload = 0x0;
        return result;
    }
    result.payload  = *((T *)((this->data)+this->getOffset(address)));
    result.valid = true;
    return result;
}

template<typename T>
bool page::write(unsigned long address, T data) {
    if((!this->safeToRead(address))||(!this->safeToRead(address+sizeof(T)-1))) {
        //not safe to write.
        return false;
    }
    *((T *)((this->data)+this->getOffset(address))) = data; //write to the location specified by the data pointer offset.
    return true;
}

template<typename T>
readResult<T> Memory::read(unsigned long address) {
    try {
        page * p = this->getPage(address);
        page * p2 = this->getPage(address+sizeof(T)-1);
        if(p!=p2) {
            //handle when it is reading across pages.
            unsigned char target[sizeof(T)]; 
            bool valid = true;
            for(int i=0; i<sizeof(T); i++) {
                page * at = this->getPage(address+i);
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
    } catch(std::exception e) {
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

template<int MAX_CHARS>
std::string Memory::stringFromMemory(unsigned long start) {
    std::string result;
    for(int i=0;i<MAX_CHARS;i++) {
        char c = this->read<char>(start+(i*INSTRUCTION_LENGTH)).payload;
        if(c=='\0') break;
        result+=c;
    }
    BOOST_LOG_TRIVIAL(debug) << "loaded string \""<<result<<"\" at: "<<start<< " with length: "<<result.length();
    return result;
}
