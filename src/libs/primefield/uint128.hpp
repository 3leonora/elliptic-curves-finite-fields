#ifndef UINT128_HPP
#define UINT128_HPP
//
// primefield/uint128.hpp
//
//      Utilities to deal with the inofficial __uint128_t type

#ifdef __clang__
// Only support 128-bit intrinsic type with clang

#include "exception.hpp"

#include <iomanip>
#include <cstdint>
#include <ostream>
#include <string.h>


// Make it possible to print out __uint128 integers
// TODO: Support io manipulators such as setfill and std::dec.
std::ostream& operator<<(std::ostream& os, __uint128_t& m);


// Make it possible to define 128 bit integers with a C++11 user defined postfix '_ulll'.
// Example:
//              auto twopow100minus1 = 1267650600228229401496703205375_ulll;
//
// Hexa-decimal is also supported:
//
//              auto twopow100minus1 = 0xfffffffffffffffffffffffff;
constexpr __uint128_t operator""_ulll(const char* str)
{
    bool hexflg = false;
    __uint128_t res = 0;
    __uint128_t base = 10;

    for (int i = 0; str[i] != '\0'; i++)
    {
        auto c = str[i];
        bool hexflg = i == 0 && c == '0';
        if (i == 1 && (c == 'x' || c == 'X')) {
            base = 16;
            res = 0;
            continue;
        }
        res *= base;

        switch (c)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                res += (c - '0');
                break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
                if (base != 16) {
                    throw primefield::LiteralSyntax("Error in __uint128_t literal (forgot 0x ?)");
                }
                res += (c - 'a' + 10);
                break;

            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':

                if (base != 16) {
                    throw primefield::LiteralSyntax("Error in __uint128_t literal (forgot 0X?)");
                }
                res += (c - 'A' + 10);
                break;

            default:
                throw primefield::LiteralSyntax("Rubbish character in __uint128_t literal");
                break;
        }
    }

    return res;
}

#endif // __clang__
#endif // UINT128_HPP
