//
// montgomery.hpp
//
// Fast helpers and primitives for doing arithmetics modulo p using the montgomery form instead.
//
// It is assumed p odd so that it becomes relative prime to 2^s.

#include <stdint>

namespace primefield
{
  template <typename UINTTYPE>
  class MontGomeryRing {
  public:
    MontGomeryRing(UINTTYPE base)
      : _base(base)
    {
      // Calculate an inverse of the the maxuint + 1 in the  ring.
    }
    
  private:
    UINTTYPE _base;
  };
}




