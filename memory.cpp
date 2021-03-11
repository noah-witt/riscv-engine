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
        result.payload = 0x0;
        return result;
    }
    result.payload  = *((this->data)+getOffset(address));
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
        result.payload = 0x0;
        return result;
    }
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
