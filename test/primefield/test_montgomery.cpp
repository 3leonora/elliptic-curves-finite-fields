#include <gtest/gtest.h>

//
// For later:
//     largest prime below 2^32: 4294967291 = 2^32 - 5
//     a large prime below 2^64: 18361375334787046697
//     primes up to 2^64 are no more than 1550 apart
//        this opens up for compact repr of list of consequtive
//        primes in a sieve!

TEST(Inverse, Montgomery) {
  EXPECT_STRNE("hello", "world");
  EXPECT_EQ(7 * 6, 42);

  
}
