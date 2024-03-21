#ifndef PRIMEFIELD_EXP_HPP
#define PRIMEFIELD_EXP_HPP

#include "traits.hpp"
#include "montgomery.hpp"

using primefield::MontgomeryField;

namespace primefield
{
    // Modular exponentiation b^e mod n
    // approx 2.5Mops / s on a Ryzen 5800X3d single thread.
    template<typename UINT>
    UINT mod_pow(UINT b, UINT e, UINT n)
    {
        using UINT2 = make_next_rank_t<UINT>;
        const UINT2 big_mod = n;

        if (n == 1)
        {
            return 0;
        }

        UINT r = 1;

        while (e > 0)
        {
            if (e & 1)
            {
                r = ((UINT2)r * (UINT2)b) % big_mod;
            }
            b = ((UINT2)b * (UINT2)b) % big_mod;
            e >>= 1;
        }
        return r;
    }

    // modular exponentiation when we know
    // b * n fits in UINT. (e < n assumed)
    // This one runs at approx 11.5 Mops / s on Ryzen 5800X3D.
    template<typename UINT>
    UINT mod_pow_lim(UINT b, UINT e, UINT n)
    {
        if (n == 1)
        {
            return 0;
        }

        UINT r = 1;

        while (e > 0)
        {
            if (e & 1)
            {
                r = (r * b) % n;
            }
            b = (b * b) % n;
            e >>= 1;
        }
        return r;
    }

}

#endif // header guard