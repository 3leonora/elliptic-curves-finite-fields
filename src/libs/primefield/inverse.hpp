#ifndef _PRIMEFIELD_INVERSE_HPP
#define _PRIMEFIELD_INVERSE_HPP

// Calculate the inverse of an element given prime field order p

#include <primefield/exception.hpp>
#include <format>

using std::format;

namespace primefield
{
  // Modular inverse according to pseudo code in
  // https://crypto.stackexchange.com/a/54477/112937
  // This solution works without the use of unsigned for
  // the whole range of an unsigned.
  template <typename UINT>
  UINT uinvert(UINT e, UINT p)
  {
      UINT a = e;
      UINT b = p;
      UINT x = 0;
      UINT y = 1;
      UINT q;

      while(true)
      {
        if (a == 1) { return y; }
        if (a == 0) { break; }

        q = b / a;
        b -= a * q;
        x += q * y;

        if (b == 1) { return p - x; }
        if (b == 0) { break; }

        q = a / b;
        a -= b * q;
        y +=  q * x;
      }

      // throw NoInverseError(format("uinverse({}, {}) non existent", e, p));
      throw NoInverseError("fail.");
  }

  // TODO: Modular inverse using the extended Euclidian algorithm on
  // https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Computing_multiplicative_inverses_in_modular_structures
  // This algorithm has to work with signed integers. 

}

#endif // _PRIMEFIELD_INVERSE_HPP
