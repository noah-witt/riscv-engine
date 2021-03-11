#include "memory.hpp"
#include <stdlib.h>

inline bool page::safeToRead(unsigned long address) {
    return PAGESIZE>address-this->address;
}

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



bool Memory::preparePage(unsigned long pageId){
    //guard make sure pageId is valid.
    if(pageId>(MAXMEMORY/PAGESIZE)) return false;
    page * p = (this->pages[pageId]);
    if(p==nullptr) {
        this->pages[pageId] = new page(pageId*PAGESIZE);
        //p = (this->pages+pageId);
    }
    //p we now know is setup validly.
    return true;
}

page * Memory::getPage(unsigned long address) {
    unsigned long pageId = address/PAGESIZE;
    if(!preparePage(pageId)) throw MEMOPOUTOFRANGE;
    //page should be readable and ready to write
    return this->pages[pageId];
}

Memory::Memory() {
    unsigned long pageCount = MAXMEMORY/PAGESIZE;
    this->pages = (page **)malloc(pageCount*sizeof(page *)); //create a pointer to a series of pages.
    for(int i=0; i<pageCount; i++) {
        this->pages[i] = nullptr;
    }
}

Memory::~Memory() {
    unsigned long pageCount = MAXMEMORY/PAGESIZE;
    for(int i=0; i<pageCount; i++) {
        if(this->pages[i]!=nullptr) {
            this->pages[i]->~page();
            free(this->pages[i]);
        }
    }
    free(this->pages);
}

readResult<unsigned char> Memory::readByte(unsigned long address) {
    try {

        page * p = getPage(address);
        return p->readByte(address);
    } catch(int e) {
        readResult<unsigned char> result;
        result.valid = false;
        result.payload = nullptr;
        return result;
    }
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
            result.payload = (T)target;
            return result;
        }
        return p->read<T>(address);
    } catch(int e) {
        readResult<T> result;
        result.valid = false;
        result.payload = nullptr;
        return result;
    }
    //should not be possible.
}

bool Memory::writeByte(unsigned long address, unsigned char data) {
    try {

        page * p = getPage(address);
        return p->writeByte(address, data);
    } catch(int e) {
        return false;
    }
    //should not be possible
}

template<typename T>
bool Memory::write(unsigned long address, T data) {
    try {

        page * p = getPage(address);
        page * p2 = getPage(address+sizeof(T)-1);
        unsigned char * list = &data;
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