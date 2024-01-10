#include <iostream>
#include <format>
#include <vector>
#include <cstdint>

using std::vector;

// Return primes up to siz
std::vector<uint64_t> basic_sieve(uint64_t siz)
{
    // No use sieving with primes higher than lim
    uint32_t lim = (int)sqrt((double)siz + 1.0);
    std::vector<bool> bits((siz+1)/2); // Skip storing bits for even numbers
    std::vector<uint64_t> primes; // Should prealloc according to some density formula...
    primes.push_back(2);
    uint64_t num_primes=1;
    uint32_t n = 3;

    uint32_t pos;
    // Main sieve loop only need to go to sqrt(siz)
    while (n <= lim) {
        pos = (n-1) >> 1;
        if (!bits[pos]) {
            // n prime. Do the sieving
            primes.push_back(n);
            uint32_t m = pos + n;
            while (m < bits.size())
            {
                bits[m] = true;
                m += n;
            }
        }
        n += 2;
    }

    // Collect the primes from rest of the sieve.
    pos++;
    while (pos < bits.size()) {
        if (!bits[pos]) {
            primes.push_back(2*pos + 1);
        }
        pos++;
    }


    return primes;
}

int main(int arc, char **argv)
{
    auto primes = basic_sieve(100);
    for (auto p: primes) {
        std::cout << p << std::endl;
    }
}