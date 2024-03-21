#ifndef PRIMEFIELD_MILLER_RABIN_HPP
#define PRIMEFIELD_MILLER_RABIN_HPP

namespace primefield
{
    // Miller-Rabin
    template<type UINT, UINT base>
    class MillerRabin
    {
        MillerRabin()
        {}

        bool operator()(UINT x)
        {
            UINT d = x - (UINT)1;
            UINT s = 0;
            while (d & 1 == 0)
            {
                d >>= 1;
                s++;
            }
            // Now 2^s * d = x - 1, d odd


        }
    };

    /*
    template<typename UINT>
    bool test_miller_rabin(UINT x, UINT base)
    {
        return MillerRabin()
    }
    */
}


#endif // PRIMEFIELD_MILLER_RABIN_HPP