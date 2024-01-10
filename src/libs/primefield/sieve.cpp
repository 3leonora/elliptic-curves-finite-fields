#include <iostream>
#include <iomanip>
#include <format>
#include <chrono>
#include <ctime>

#include <vector>
#include <cstdint>
#include <cmath>

using std::vector;
using std::cout;
using std::endl;
using std::sqrt;

// For p = 2**38 < 304,599,508,537 < 2**39 the gaps are bigger than 512 requiring
// additive storing of primes larger than uint8_t.

// 2**64 ~< 2*1e19. pi(1e19) ~ 2.3e17
// 2**40 ~> 1e12.   pi(1e12) ~ 3.8e10
// 2**20 ~> 1e6.    pi(1e6) = 78,498. : Storing these < 100M. Differential storing 1 byte / prime.

static std::string now()
{
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%F %T");
    return oss.str();
}

// Calculate pi(n) for n=2**s up to 2**40.
std::vector<uint8_t> basic_sieve40(uint64_t siz)
{
    cout << now() << std::format(": Start basic_sieve40({})", siz) << endl;
    // No use sieving with primes higher than lim
    uint32_t lim = (int)sqrt((double)siz + 1.0);
    std::vector<bool> bits((siz+1)/2); // Skip storing bits for even numbers
    std::vector<uint8_t> primegaps; // Should prealloc according to some density formula...
    primegaps.push_back(2);
    uint64_t num_primes=1;
    
    // Remove later. For writing out different values of pi to verify against 
    // https://en.wikipedia.org/wiki/Prime-counting_function

    uint64_t pi_check = 10; // next 10-power to output pi(n) for.

    uint64_t n = 3; // Next number to try
    uint64_t latest_prime = 2;
    uint64_t pos;
    // Main sieve loop only need to go to sqrt(siz)
    while (n <= lim) {
        pos = (n-1) >> 1;
        if (!bits[pos]) {
            // Next prime found.
            if (n > pi_check) {
                std::cout << now() << ": " << std::format("pi({}) = {} ({})", pi_check, num_primes, latest_prime) << endl;
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

    // Collect the primes from rest of the sieve.
    pos++;
    while (pos < bits.size()) {
        if (!bits[pos]) {
            n = 2*pos + 1;
            if (n > pi_check) {
                std::cout << now() << ": " << std::format("pi({}) = {} ({})", pi_check, num_primes, latest_prime) << endl;
                pi_check *= 10;
            }            
            num_primes++;
            primegaps.push_back(n - latest_prime);
            latest_prime = n;  
        }
        pos++;
    }


    return primegaps;
}

int main(int arc, char **argv)
{
    auto gaps = basic_sieve40(10'000'100'000);
    /*
    uint64_t p = 0;
    for (auto gap: gaps) {
        p += gap;
        std::cout << p << std::endl;
    }
    */
}