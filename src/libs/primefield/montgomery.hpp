#ifndef PRIMEFIELD_MONTGOMERY_HPP
#define PRIMEFIELD_MONTGOMERY_HPP
//
// montgomery.hpp
//
// Fast helpers and primitives for doing arithmetics modulo p using the montgomery form instead.
//
// It is assumed p odd so that it becomes relative prime to 2^s.

#include "inverse.hpp"
#include "traits.hpp"
#include <sstream>

#include <cstdint>

// The type given will implicitly define R = 2**s; uint32_r -> R = 2**32.
namespace primefield
{
  template<typename UINT>
  class MontgomeryField {
  public:

    using UINT2 = make_next_rank_t<UINT>;

    // ctor
    MontgomeryField(UINT base);

    UINT form(UINT e);  // Calculate the Montgomery form e * 2^s mod base

    UINT _N;    // The modulo we work in
    UINT _Np;   // N * Np = -1 mod R
    UINT _Rinv; // The inverse mod _base of the 2^s (maxint+1)
  };


  // Method implementations below

  // Constructor
  template <typename UINT>
  MontgomeryField<UINT>::MontgomeryField(UINT base)
  : _N(base)
  {
      // Calculate an inverse of the the maxuint + 1 in the  ring.
      _Rinv = primefield::uinvert((UINT)((UINT)0 - _N), _N);
      // Calculate _Np such that N * Np = -1 mod R. This is used in the fast REDC algorithm
      _Np = primefield::uinvert(base);
  }

  // TODO:
  //    Should be possible to do without need of larger type?
  template <typename UINT>
  UINT MontgomeryField<UINT>::form(UINT e)
  {
    using UINT2 = make_next_rank_t<UINT>;
    UINT2 t = e << ((UINT)8 * sizeof(UINT));
    return t % _N;
  }


}


#endif // PRIMEFIELD_MONTGOMERY_HPP

