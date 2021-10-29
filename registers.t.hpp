#pragma once

#include "./registers.hpp"
#include <boost/log/trivial.hpp>

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
    if constexpr(! std::is_same<a, void>::value) {
        result[0] = (a *)(at);
        at=(void *)(((a *)at)+1);
    }
    if constexpr(! std::is_same<b, void>::value) {
        result[1] = (b *)(at);
        at=(void *)(((b *)at)+1);
    }
    if constexpr(! std::is_same<c, void>::value) {
        result[2] = (c *)(at);
        at=(void *)(((c *)at)+1);
    }
    if constexpr(! std::is_same<d, void>::value) {
        result[3] = (d *)(at);
        at=(void *)(((d *)at)+1);
    }
    return result;
}

template<typename a, typename b, typename c, typename d>
std::array<long, 4> Register::readInstructionNormalized() {
    std::array<void*,4> parts = this->readInstruction<a,b,c,d>();
    std::array<long, 4> result;
    if constexpr(! std::is_same<a, void>::value) {
        result[0] = *((a *)parts[0]);
    } else {
        result[0] = -1;
    }
    if constexpr(! std::is_same<b, void>::value) {
        result[1] = *((b *)parts[1]);
    } else {
        result[1] = -1;
    }
    if constexpr(! std::is_same<c, void>::value) {
        result[2] = *((c *)parts[2]);
    } else {
        result[2] = -1;
    }
    if constexpr(! std::is_same<d, void>::value) {
        result[3] = *((d *)parts[3]);
    } else {
        result[3] = -1;
    }
    return result;
}

template<typename resultType, int startOffsetBytes, bool neg>
resultType Register::read() {
    uint8_t * byteptr = (uint8_t *) this->value;
    byteptr+=(neg?-1:1)*startOffsetBytes;
    return *((resultType*)byteptr);
}

template<typename inputType, int startOffsetBytes, bool neg>
void Register::write(const inputType& in) {
    if(! this->isMutable) {
        // this is an immutable register.
        return;
    }
    uint8_t * byteptr = (uint8_t *) this->value;
    byteptr+=(neg?-1:1)*startOffsetBytes;
    *((inputType *)byteptr) = in;
}

template<typename inputType>
void Register::writeLower(const inputType& in) {
    this->write<inputType, REGISTER_WIDTH_BYTES-sizeof(inputType)>(in);
}

template<typename resultType>
resultType Register::readLower() {
    return this->read<resultType, REGISTER_WIDTH_BYTES-sizeof(resultType)>();
}

// some manual instantiations to correct some linking issues.
template void Register::writeInstruction<unsigned short, unsigned char, unsigned char, unsigned int>(unsigned short const&, unsigned char const&, unsigned char const&, unsigned int const&);
template void Register::writeInstruction<unsigned short, unsigned short, unsigned short, unsigned short>(unsigned short const&, unsigned short const&, unsigned short const&, unsigned short const&);
template unsigned long Register::read<unsigned long>();
template void Register::write<unsigned long>(unsigned long const&);

template std::array<void*, 4ul> Register::readInstruction<unsigned short, void, void, void>();
template std::array<void*, 4ul> Register::readInstruction<unsigned short, unsigned short, unsigned short, unsigned short>();
template std::array<void*, 4ul> Register::readInstruction<unsigned short, unsigned char, unsigned char, int>();
template int Register::read<int, 0, false>();
template void Register::write<int, 0, false>(int const&);
template void Register::write<long, 0, false>(long const&);
template void Register::writeInstruction<unsigned short, unsigned char, unsigned char, int>(unsigned short const&, unsigned char const&, unsigned char const&, int const&);
