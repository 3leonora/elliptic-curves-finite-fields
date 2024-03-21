#ifndef PRIMEFIELD_INVERSE_HPP
#define PRIMEFIELD_INVERSE_HPP

// Calculate the inverse of an element given prime field order p

#include <primefield/exception.hpp>
#include <primefield/uint128.hpp>
#include <limits>
#include <sstream>

namespace primefield
{
  // Modular inverse according to pseudo code in
  // https://crypto.stackexchange.com/a/54477/112937
  // This solution works without the use of signed for
  // the whole range of an unsigned.
  template <typename UINT>
  UINT uinvert(UINT e, UINT N)
  {
      UINT a = e;
      UINT b = N;
      UINT x = 0;
      UINT y = 1;
      UINT q;

      while(true)
      {
        if (a == 1) { return y; }
        if (a == 0) { break; } // -> error

        q = b / a;
        b -= a * q;
        x += q * y;

        if (b == 1) { return N - x; }
        if (b == 0) { break; } // -> error

        q = a / b;
        a -= b * q;
        y +=  q * x;
      }

      std::ostringstream errmsg;
      errmsg << "uinverse(" << e << ", " << N << ") non existent";
      throw NoInverseError(errmsg.str());
  }

  // Find inverse of odd numbers modulo 2**s where 2**s-1 is maximal value of UINT
  // This version avoids using higher rank types.
  template <typename UINT>
  UINT uinvert(UINT e)
  {
      UINT a = e;
      UINT b;
      UINT x = 0;
      UINT y = 1;
      UINT q;

      if (a == 1) {return y;}
      if (a == 0) {throw NoInverseError("Trying to invert 0");}

      q = std::numeric_limits<UINT>::max() / a;
      b = -(a * q);
      x += q * y;

      while(true)
      {
        if (b == 1) { return -x; }
        if (b == 0) { break; } // -> error

        q = a / b;
        a -= b * q;
        y +=  q * x;

        if (a == 1) { return y; }
        if (a == 0) { break; } // -> error

        q = b / a;
        b -= a * q;
        x += q * y;
      }

      std::ostringstream errmsg;
      errmsg << "uinverse(" << e << ") no inverse mod " << 1 + std::numeric_limits<UINT>::max();
      throw NoInverseError(errmsg.str());
  }

  // Another algorithm for modular inverse of odd numbers in 2^64 modulo
  // (Newton's method)
  // See: https://lemire.me/blog/2017/09/18/computing-the-inverse-of-odd-integers/
  // This seems at least 15x quicker than the uinvert.
  // TODO: generalize this and replace the uinvert above...

  inline uint64_t f64(uint64_t x, uint64_t y)
  {
      return y * (2 - y * x);
  }

  inline uint64_t findInverse64(uint64_t x)
  {
      uint64_t y = (3 * x) ^ 2; // 5 bits
      y = f64(x, y);            // 10 bits
      y = f64(x, y);            // 20 bits
      y = f64(x, y);            // 40 bits
      y = f64(x, y);            // 80 bits
      return y;
  }

  // TODO: Modular inverse using the extended Euclidian algorithm on
  // https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Computing_multiplicative_inverses_in_modular_structures
  // This algorithm has to work with signed integers.
}

#endif // PRIMEFIELD_INVERSE_HPP
