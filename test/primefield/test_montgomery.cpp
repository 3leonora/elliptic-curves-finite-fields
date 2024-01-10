
#include <primefield/montgomery.hpp>

#include <gtest/gtest.h>

#include <cstdint>
#include <format>
#include <ios>

using primefield::MontgomeryField;

//
// For later:
//     largest prime below 2^32: 4294967291 = 2^32 - 5
//     a large prime below 2^64: 18361375334787046697
//     primes up to 2^64 are no more than 1550 apart
//        this opens up for compact repr of list of consequtive
//        primes in a sieve!

TEST(Montgomery, Basic) {
  uint16_t p = 19;
  auto mg = MontgomeryField(p);
  for (int i=1; i<p; i++) {
    for (int j=1; j<p; j++) {
      EXPECT_EQ(
        (mg.form(i) * mg.form(j) * mg._Rinv) % p,
        mg.form(i * j)
      );
    }
  }


}
