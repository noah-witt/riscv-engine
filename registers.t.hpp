#pragma once

#include "./registers.hpp"

template<typename a, typename b, typename c, typename d>
void Register::writeInstruction(const a& va, const b& vb, const c& vc, const d& vd) {
    if(! std::is_same<a, void>::value) {
        this->write<a>(va);
    }
    if(! std::is_same<b, void>::value) {
        this->write<b, sizeof(a)>(vb);
    }
    if(! std::is_same<c, void>::value) {
        this->write<c, sizeof(a)+sizeof(b)>(vc);
    }
    if(! std::is_same<d, void>::value) {
        this->write<d, sizeof(a)+sizeof(b)+sizeof(c)>(vd);
    }
}

template<typename a, typename b, typename c, typename d>
std::array<void*,4> Register::readInstruction() {
    std::array<void*,4> result;
    void * at = (void *)this->value;
    if(! std::is_same<a, void>::value) {
        result[0] = (a *)(at);
        at=(void *)(((a *)at)+1);
    }
    if(! std::is_same<b, void>::value) {
        result[1] = (b *)(at);
        at=(void *)(((b *)at)+1);
    }
    if(! std::is_same<c, void>::value) {
        result[2] = (c *)(at);
        at=(void *)(((c *)at)+1);
    }
    if(! std::is_same<d, void>::value) {
        result[3] = (d *)(at);
        at=(void *)(((d *)at)+1);
    }
    return result;
}

template<typename resultType, int startOffsetBytes>
resultType * Register::read() {
    uint8_t * byteptr = (uint8_t *) this->value;
    byteptr+=startOffsetBytes;
    return (resultType *)byteptr;
}

template<typename inputType, int startOffsetBytes>
void Register::write(const inputType& in) {
    if(! this->isMutable) {
        // this is an immutable register.
        return;
    }
    uint8_t * byteptr = (uint8_t *) this->value;
    byteptr+=startOffsetBytes;
    *((inputType *)byteptr) = in;
}

template<typename inputType>
void Register::writeLower(const inputType& in) {
    this->write<inputType, REGISTER_WIDTH_BYTES-sizeof(inputType)>(in);
}

template<typename resultType>
resultType* Register::readLower() {
    return this->read<resultType, REGISTER_WIDTH_BYTES-sizeof(resultType)>();
}

template void Register::writeInstruction<unsigned short, unsigned char, unsigned char, unsigned int>(unsigned short const&, unsigned char const&, unsigned char const&, unsigned int const&);
template void Register::writeInstruction<unsigned short, unsigned short, unsigned short, unsigned short>(unsigned short const&, unsigned short const&, unsigned short const&, unsigned short const&);
template unsigned long* Register::read<unsigned long, 0>();
template void Register::write<unsigned long, 0>(unsigned long const&);
