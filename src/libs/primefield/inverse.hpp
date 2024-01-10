#ifndef PRIMEFIELD_INVERSE_HPP
#define PRIMEFIELD_INVERSE_HPP

// Calculate the inverse of an element given prime field order p

#include <primefield/exception.hpp>
#include <format>
#include <limits>

using std::format;

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

      throw NoInverseError(format("uinverse({}, {}) non existent", e, N));
  }

  // Find inverse of odd numbers module 2**s where 2**s-1 is maximal value of UINT
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

      throw NoInverseError(format("uinverse({}) no inverse mod {}", e, 1 + std::numeric_limits<UINT>::max()));
  }

  // TODO: Modular inverse using the extended Euclidian algorithm on
  // https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Computing_multiplicative_inverses_in_modular_structures
  // This algorithm has to work with signed integers. 

}

#endif // PRIMEFIELD_INVERSE_HPP
