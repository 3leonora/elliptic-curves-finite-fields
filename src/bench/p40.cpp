#include <iostream>
#include <sstream>
#include <iomanip>
#include <format>
#include <vector>
#include <tuple>
#include <bitset>
#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include <cmath>

#include <profiling/logger.hpp>
#include <hurchalla/modular_arithmetic/modular_multiplicative_inverse.h>

using std::vector;
using std::min;
using std::max;
using std::cout;
using std::endl;
using std::sqrt;
using profiling::Logger;
using std::tie;
using Oss = std::ostringstream;
using Vec16 = vector<uint16_t>;
using hurchalla::modular_multiplicative_inverse;

const uint64_t SegSize = 2'000'000ULL;

// Provide an upper bound for pi(n)
// See https://en.wikipedia.org/wiki/Prime-counting_function#Inequalities
static uint64_t
pi_upper(uint64_t n)
{
    double n_f = n;
    if (n < 60184) {
        return ceil(1.25506 * n_f / log(n_f));
    }
    else {
        // Pierre Dusart, 2010
        return ceil(n_f / (log(n_f) - 1.1));
    }
}

// Note regarding finding using Miller-Rabin:
// There are approx 996 pseudo primes to base 2,3. below 2^40.
//        https://oeis.org/A072276/b072276.txt
// These could
// easily be hashed. Is it quicker searching for primes using this test?


// For p = 2**38 < 304,599,508,537 < 2**39 the gaps might be bigger than 512 requiring
// differential storing of primes larger than uint8_t.

// 2**64 ~< 2*1e19. pi(1e19) ~ 2.3e17
// 2**40 ~> 1e12.   pi(1e12) ~ 3.8e10
// 2**20 ~> 1e6.    pi(1e6) = 78'498. : Storing these < 100K. Differential storing 1 byte / prime.
// pi(2**40) = 41'203'299'196 (24 minutes single thread Ryz 5800X3D, seg size 1e6)

// siz[in]          Check all integers up to this
// last_prime[out]  Largest prime < siz
// @returns vector of prime gap sizes. First value will be 2.
std::vector<uint16_t> basic_sieve40(uint64_t siz, uint64_t& last_prime, Logger& logger)
{
    logger.format("basic_sieve({})", siz);
    // No use sieving with primes higher than lim
    uint32_t lim = (int)sqrt((double)siz + 1.0);
    std::vector<bool> bits((siz+1)/2); // Skip storing bits for even numbers
    std::vector<uint16_t> primegaps;
    primegaps.reserve(pi_upper(lim));
    primegaps.push_back(2);
    uint64_t num_primes=1;

    // Remove later. For writing out different values of pi to verify against
    // https://en.wikipedia.org/wiki/Prime-counting_function

    uint64_t pi_check = 10; // next 10-power to output pi(n) for.

    uint64_t n = 3; // Next number to try
    last_prime = 2;
    uint64_t pos;
    // Initial sieve loop only need to go to sqrt(siz)
    while (n <= lim) {
        pos = (n-1) >> 1;
        if (!bits[pos]) {
            // Next prime found.
            if (n > pi_check) {
                logger.format("pi({:.0e}) = {} ({})", (double)pi_check, num_primes, last_prime);
                pi_check *= 10;
            }
            num_primes++;
            // n prime. Do the sieving
            primegaps.push_back(n - last_prime);
            last_prime = n;

            // Strike over all multiples in rest of bitmap. "The sieve".
            uint64_t m = pos + n;
            while (m < bits.size())
            {
                bits[m] = true;
                m += n;
            }
        }
        n += 2;
    }

    logger.format("At {}. Proceeding with just extracting primes", n);

    // Collect the primes from rest of the sieve.
    pos++;
    while (pos < bits.size()) {
        if (!bits[pos]) {
            n = 2*pos + 1;
            if (n > pi_check) {
                logger.format("pi({:.0e}) = {} ({})", (double)pi_check, num_primes, last_prime);
                pi_check *= 10;
            }
            num_primes++;
            primegaps.push_back(n - last_prime);
            last_prime = n;
        }
        pos++;
    }

    logger.format("Finish. Last prime: {}", last_prime);

    return primegaps;
}

// Create a differential vector of prime candidates given the first n primes
// n[in]     number of primes (starting with 2)
// next_prime[out] Next prime not part of wheel forming primes
// primorial[out]  product of the first primes = size of wheel
vector<uint32_t> create_wheel(uint64_t n, uint64_t& last_prime, uint64_t& primorial, Logger& logger)
{
    // Brute force a big enough basic sieve.
    uint64_t m = 100;
    while (pi_upper(m) < n)
    {
        m += 100;
    }
    logger.format("Sieving {} primes from (2, {})", n, m);
    uint64_t high_prime;
    auto gaps = basic_sieve40(m, high_prime, logger);

    // degap to store the real primes
    // Also calculate product (= size of wheel)
    std::vector<uint64_t> primes;
    primes.reserve(n);
    uint64_t p = 0;
    primorial = 1;
    uint32_t nwheel = 1; // prod(p_i - 1);
    Oss ptxt;
    for (int i = 0; i < n; i++)
    {
        p += gaps[i];
        ptxt << " " << p;
        primes.push_back(p);
        primorial *= p;
        nwheel *= p-1;
    }
    logger.format("Wheel({}) with primorial {} and size {}", ptxt.str(), primorial, nwheel);
    last_prime = p;

    std::vector<bool> sieve(primorial, false);
    for (auto prim: primes)
    {
        for (uint64_t t = prim; t < primorial; t += prim)
        {
            sieve[t] = true;
        }
    }

    sieve[0] = true; // First element dividable with all the primes!

    // All still false will be prime candidates within the repeated
    // intervals

    std::vector<uint32_t> wheel;
    wheel.reserve(nwheel);
    for (uint32_t i = 0; i < primorial; i++)
    {
        if (!sieve[i])
        {
            wheel.push_back((uint32_t)i);
        }
    }

    return wheel;
}

Vec16 segmented_sieve40(uint64_t siz, Logger& logger)
{
    // No use sieving with primes higher than lim
    uint32_t lim = (int)sqrt((double)siz + 1.0);
    std::vector<bool> bits((lim+1)/2); // /2 --> Skip storing bits for even numbers
    Vec16 primegaps;
    uint64_t ulprimes = pi_upper(lim);
    logger.format("Upper limit on number of primes < sqrt(n): {}", ulprimes);
    primegaps.reserve(ulprimes);
    primegaps.push_back(2); // First prime's offset to 0
    uint64_t num_primes=1;

    // Remove later. For writing out different values of pi to verify against
    // https://en.wikipedia.org/wiki/Prime-counting_function
    uint64_t pi_check = 10; // next 10-power to output pi(n) for.

    uint64_t n = 3; // Next number to try
    uint64_t latest_prime = 2;
    uint64_t pos;
    // Initial sieve loop only need to go to sqrt(siz)
    logger.format("Starting basic sieve up to {}", lim);
    while (n <= lim) {
        pos = (n-1) >> 1;
        if (!bits[pos]) {
            // Next prime found.
            if (n > pi_check) {
                logger.format("pi({:.0e}) = {} ({})", (double)pi_check, num_primes, latest_prime);
                pi_check *= 10;
            }
            num_primes++;
            // n prime. Do the sieving
            primegaps.push_back(n - latest_prime);
            latest_prime = n;

            // Strike over all multiples in rest of bitmap. "The sieve".
            uint64_t m = pos + n;
            while (m < bits.size())
            {
                bits[m] = true;
                m += n;
            }
        }
        n += 2;
    }

    logger.format("Sieving primes vector done. Size = {}, Highest prime = {}",
        primegaps.size(), latest_prime);

    // Use primes up to lim to sieve segments ahead
    vector<bool> segbits(SegSize); // vector<bool> strangely seems faster than std::bitset.

    uint64_t sieve_size = primegaps.size();
    logger.format("Starting segmented sieve with seg size {}", segbits.size());

    uint64_t low = n; // an odd number
    uint64_t high = n + segbits.size();
    //    vector<bool> segbits(SegBitSet.size(), false);

    while (low < siz)
    {
        // logger.format("Running on segment ({},{}( (siz = {})", low, high, siz);
        if (high >= siz)
        {
            high = siz;
        }

        std::fill(segbits.begin(), segbits.end(), false);

        // Loop over all the first basic sieved primes.
        // We skip p = 2 since we will not consider the even
        // numbers in the segment.
        uint64_t p = 2;

        uint64_t up_to = (int)sqrt((double)(high+1)) + 1;
        int i = 1;
        while (true)
        {
            p += primegaps[i++];
            if (p >= up_to) break;
            // logger.format("Checking interval {} against prime {}", low, p);

            // Derive the first multiple of p that is low or higher
            // and where the multiple is odd
            uint64_t mp = (low / p) * p;
            if (mp % 2 == 0) // Check if even.
            {
                mp += p;
            }

            if (mp < low)
            {
                mp += 2*p;
            }

            for (; mp < high; mp += 2*p)
            {
                segbits[mp - low] = true;
            }
        }

        // Scramble together all still true bits representing values below the chosen siz.
        for (uint64_t i=0; i < segbits.size(); i += 2)
        {

            if (!segbits[i])
            {
                // Found the prime low+i
                p = low + i;
                if (p > siz) break;
                // logger.format("Found prime: {}", p);
                if (p > pi_check) {
                    logger.format("pi({:.0e}) = {} ({})", (double)pi_check, num_primes, latest_prime);
                    pi_check *= 10;
                }
            num_primes++;
            //primegaps.push_back(p - latest_prime);
            latest_prime = p;
            }

        }

        low += segbits.size();
        high += segbits.size();

    } // while (low < siz)
    logger.format("Finish. Total number of primes found: {}", num_primes);

    return primegaps;
}

// What to do with each prime we find!?
class Inspect1
{
    // Calculate pi(2^n) for some n:s. See OEIS sequence A007053
    // https://oeis.org/A007053
    uint64_t highest = 0ull;
    uint64_t count = 0ull;
    uint64_t pi_1e11_count = 0ull; // Count primes below 10^11
    uint64_t pi_1e12_count = 0ull; // Count primes below 10^12
    uint64_t pi_2p40_count = 0ull; // Count primes below 2^40
    uint64_t pi_2p40_max = 0ull; // Count primes below 2^40
    uint64_t count_test = 0ull;
    uint64_t max_test = 0ull;
    uint64_t p2counters[65] = {0,};

    public:

    void check(uint64_t p, Logger& logger)
    {
        p2counters[__lzcnt64(p)]++;
        /*
        if (p > highest) highest = p;
        if (p < 100'000'000'000ull) pi_1e11_count++;
        if (p < 1000'000'000'000ull) pi_1e12_count++;

        if (p < 1ull<<40ull) {
            pi_2p40_count++;
            if (p > pi_2p40_max) pi_2p40_max = p;
        }
        /*if (p > 3'000'000 and p < 1ull << 40ull)
        {
            count_test++;
            if (p > max_test) max_test = p;
        }*/
        //count++;
        // if (p > highest) highest = p;
    }

    void show(Logger& logger)
    {
        // logger.format("--- Total count = {}, highest = {}", count, highest);
        // logger.format("pi(1e11) = {}", pi_1e11_count);
        // logger.format("pi(1e12) = {}", pi_1e12_count);
        //  logger.format("pi(2^40) = {}, max = {}", pi_2p40_count, pi_2p40_max);
        // logger.format("test count = {}, max = {}", count_test, max_test);

        // '2' counted in the "wrong" slot. Adjust.
        p2counters[63]++;
        p2counters[62]--;
        uint64_t acc = 0;
        int i = 63;
        while (p2counters[i] != 0)
        {
            acc += p2counters[i];
            logger.format("pi(2^{}) = {}", 64 - i, acc);
            i--;
        }
    }
};


Vec16 segwheel_sieve40(uint64_t siz, Logger& logger)
{
    // Construct prime wheel (candidates for being prime)
    uint64_t primorial;
    uint64_t start_prime; // Next prime after wheel primes.
    const uint32_t wheel_n = 5; // Number of primes to base wheel on.
    auto wheel = create_wheel(wheel_n, start_prime, primorial, logger);
    Oss oss;
    uint64_t p = 0;
    uint32_t lim = (uint32_t)sqrt((double)siz + 1.0) + 1u;

    // Instantiate the work we do with the primes.
    Inspect1 checker;

    // Initial sieve loop only need to go to sqrt(siz)
    logger.format("Starting basic sieve up to {}", lim);
    uint64_t high_prime;
    auto primegaps = basic_sieve40(lim, high_prime, logger);

    uint64_t primes_found = primegaps.size();

    // Calculate the factors to retrieve the 2nd level residue class of the prime
    // being checked in the wheel residue class r. At the same time process the
    // primes.

    vector<uint32_t> rfactors;
    rfactors.reserve(primegaps.size());
    uint32_t p32 = 0;
    for (auto d: primegaps)
    {
        uint32_t tmpgcd;
        p32 += d;
        rfactors.push_back(p32 - modular_multiplicative_inverse<uint32_t>(primorial, p32, tmpgcd));
        //if (rfactors.size() < 10)
        //    logger.format("rfac({}) = {}", p32, rfactors.back());
        checker.check(p32, logger);
    }


    // Output the 2nd level residues given the first 10 residue classes and start_prime
    p32 = 0;
    // logger.format("Some 2nd level residues for r = 1");
    for (int i=0; i<20; i++) {
        p32 += primegaps[i];
        // logger.format("p = {} | {} + [{}]*{}", p32, wheel[0], primorial, (rfactors[i] * wheel[0]) % p32);
    }

    // Construct the segment bit mask.
    const uint32_t numsegwords32 = 1ul<<16ul; // -> 2^21 ~ 2e6 bits.
    const uint64_t numsegbits = 32 * numsegwords32;
    auto *segbits = new uint32_t[numsegwords32];
    //vector<bool> segbits_old(2000000); // 1e6 seems to be a good segment size.
    logger.format("Start of segmented sieve phase using {} primes. Segment size: {}", primegaps.size(), numsegbits);

    // We only test the residue classes as indicated in the wheel.
    for (auto r: wheel)
    {
        // Split the residue class into equally sized segments. Each bit represent
        // the number r + n * primorial; n = 0, 1, 2,...

        // Split the residue class into segments. We need to be careful to
        // start at the lowest possible n such that n*primorial + r >= lim.
        // uint64_t low = (lim - r + primorial - 1) / primorial;
        // uint64_t low = (lim - r) / primorial;
        uint64_t low = 1;
        logger.format("Starting sieve in {}:{} on index {}", primorial, r, low);
        // low_last shall be the the first multiple where low_last * primorial + r >= siz
        uint64_t low_last = (siz - r + 1) / (uint64_t)primorial;
        if (low * primorial + r > lim) {
            logger.format("ERROR - low wrongly calculated.");
            continue;
        }
        // else if (low * primorial + r < lim - primorial)
        //{
        //    logger.format("ERROR - low calculated too small");
        //    continue;
        //}

        // while (low * (uint64_t)primorial < siz)
        // while (low <= low_last + 1)
        while (low * (uint64_t)primorial + r < siz)
        {
            // logger.format("Running residue {}:{} segment {}-{}", primorial, r, low, low + numsegbits);

            std::memset(segbits, 0, sizeof(uint32_t) * numsegwords32);
            // std::fill(segbits.begin(), segbits.end(), false);
            // Now loop over all relevant non-wheel integers we have
            uint32_t seg_sqrt = sqrt((double)primorial * (double)(low + numsegbits));
            // Could reduce the loop below up_to = (int)sqrt((double)(high+1)) + 1;
            uint32_t ix = wheel_n;
            //for (uint32_t i = wheel_n; i < primegaps.size(); i++)

            // Loop over all primes in sieve-set
            p32 = start_prime;
            for (uint32_t ix = wheel_n; ix < primegaps.size(); ix++)
            {
                p32 += primegaps[ix];
                // Calculate where prime marking will start (0, p32-1)
                uint64_t a1 = (uint64_t)rfactors[ix] * (uint64_t)r;
                uint32_t offs;
                if (a1 > low) {
                    offs = (a1 - low) % p32;
                }
                else {
                    offs = p32 - (low - a1) % p32;
                    if (offs == p32) offs = 0;
                }

                while (offs < numsegbits)
                {
                    segbits[offs / 32ul] |= 0x1ul << (offs % 32ul); // Mark as composite.
                    offs += p32;
                }
            }

            // Collect values still marked false in the sieve segment. Those are primes
            uint64_t p64_segstart = low * primorial + (uint64_t)r;
            uint64_t p64 = p64_segstart;
            uint32_t num_full_words = numsegwords32;
            if (p64 + primorial * numsegbits >= siz) {
                // Short down the range of this last segment
                num_full_words = (siz - p64) / (32ull * (uint64_t)primorial);
            }
            uint32_t wordix;
            for (wordix = 0; wordix < num_full_words; wordix++)
            {
                uint32_t entry = segbits[wordix];
                uint32_t primorial2 = 2ul * primorial;
                for (uint32_t j = 0; j < 16; j++)
                {
                    switch (entry & 3ul) // 3% speed up by treating 2 bits at a time...
                    {
                    case 2:
                        checker.check(p64, logger);
                    case 3:
                        p64 += primorial2;
                        break;
                    case 0:
                        checker.check(p64, logger);
                    case 1:
                        p64 += primorial;
                        checker.check(p64, logger);
                        p64 += primorial;
                        break;
                    }
                    entry >>= 2ul;
                }
            }

            if (wordix < numsegwords32) {
                // Apparently at last segment with a partial word still to check.
                // Validate until the prime candidate is >= siz.
                uint32_t entry = segbits[wordix];
                while (p64 < siz)
                {
                    if (!(entry & 0x1)) checker.check(p64, logger);
                    p64 += primorial;
                    entry >>= 1ul;
                }
            }

            // low += segbits_old.size();
            low += numsegbits;
        }
    }

    checker.show(logger);

    return primegaps;
}


int main(int arc, char **argv)
{
    // const uint64_t max_n = 100'000'100'000;
    const uint64_t max_n = (1ull<<40ull); // + (1ull<<30ull);
    Logger logger("p40");
    logger.format("Enumerating primes < {}. p(n) < {}", max_n, pi_upper(max_n));
    //auto gaps1 = basic_sieve40(max_n, logger);
    // logger.reset("segmented sieve");
    // auto gaps2 = segmented_sieve40(max_n, logger);
    auto gaps3 = segwheel_sieve40(max_n, logger);
    /*
    uint64_t p = 0;
    for (auto gap: gaps) {
        p += gap;
        std::cout << p << endl;
    }
    */
}