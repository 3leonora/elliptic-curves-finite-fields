#ifdef __clang__
#include "uint128.hpp"

std::ostream& operator<<(std::ostream& os, __uint128_t& m)
{
    os  << "0x"
        << std::hex << std::setfill('0') << std::setw(16)
        << (uint64_t)(m >> 64)
        << std::hex << std::setfill('0') << std::setw(16)
        << (uint64_t)(m & 0xffffffffffffffffULL);
    return os;
}
#endif
