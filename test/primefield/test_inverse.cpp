#include <primefield/traits.hpp>
#include <primefield/inverse.hpp>
#include <primefield/uint128.hpp>

#include <gtest/gtest.h>
#include <ios>
#include <format>

using primefield::uinvert;
using primefield::findInverse64;
using primefield::make_next_rank_t;
using std::format;

TEST(Inverse, BasicUInvert)
{
  EXPECT_EQ(uinvert(14, 31), 20);
  EXPECT_EQ(uinvert(2U, 4294967291U), 2147483646U);
  // Needed the cast below on Windows clang-cl (ver. 16)
  EXPECT_EQ(uinvert((uint64_t)2UL, (uint64_t)18361375334787046697UL), 9180687667393523349UL);
  EXPECT_THROW(uinvert(6, 21), primefield::NoInverseError);
}


// Test the 2**s inverses of 16-bit odd numbers
TEST(Inverse, TwoPowInverse)
{
  EXPECT_EQ(uinvert((uint16_t)1), 1);

  for (uint16_t i=3; i != 1; i += 2)
  {
    uint16_t j = uinvert(i);
    ASSERT_EQ((uint16_t)(i * j), (uint16_t)1) << format("uinvert({}) = {}", i, j);
  }
}


#ifdef __clang__
TEST(Inverse, CanHandle128BitInteger)
{
  // No literal support for 128 bit integers. We could develop one ourself...
  // __uint128_t m = 0xffffffffffffffffULL;
  // m *= m; // Now a very large odd number relative prime to the 16 below we try to invert.
  auto m = 0xfffffffffffffffe0000000000000001_ulll;
  EXPECT_EQ(uinvert((__uint128_t)16, m), 0xeffffffffffffffe2000000000000001_ulll);

  // Test some more of the literal logic.
  EXPECT_EQ(12345, 12345_ulll);
  EXPECT_EQ(0x1234abcd, 0x1234abcd_ulll);
  EXPECT_EQ(0x1234abcd, 0X1234abcd_ulll);
  EXPECT_EQ(0x1234abcd, 0X1234AbCd_ulll);
}
#endif


TEST(Inverse, UInvertExtensive)
{
  using Uxx = uint16_t;

  // Test inverse of powers of 2 in all odd modulo groups U16.
  for (Uxx m = 3; m > 2; m += 2)
  {
    for (make_next_rank_t<Uxx> e = 2; e < m; e *= 2)
    {
      make_next_rank_t<Uxx> e_inv = uinvert((Uxx)e, m);
      ASSERT_EQ((e_inv * e) % m, 1);
    }
  }

  // Test inverse of all odd elements < 2^15 modulo 2^15
  Uxx m = (Uxx)1 << 15;
  for (Uxx e = 3; e < m; e += 2)
  {
    make_next_rank_t<Uxx> e_inv = uinvert((Uxx)e, m);
    ASSERT_EQ((e_inv * e) % m, 1);
  }
}

TEST(Inverse, OddInverse64_1)
{
    // Test the uinvert for 64-bit
    using Uxx = uint64_t;
    Uxx sum = 0;

    for (Uxx e = 3; e < 100'000'000; e += 2)
    {
        Uxx e_inv = uinvert(e);
        sum += e_inv;
        // ASSERT_EQ(1, e_inv * e);
    }
    ASSERT_EQ(sum, 12219767026090819583LLU);
}

TEST(Inverse, OddInverse64_2)
{
    // Test the newton  for 64-bit
    using Uxx = uint64_t;
    Uxx sum = 0;

    for (Uxx e = 3; e < 100'000'000; e += 2)
    {
        Uxx e_inv = findInverse64(e);
        sum += e_inv;
        // ASSERT_EQ(1, e_inv * e);
    }
    ASSERT_EQ(sum, 12219767026090819583LLU);
}

