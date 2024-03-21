#include <primefield/traits.hpp>
#include <primefield/exp.hpp>

#include <hurchalla/montgomery_arithmetic/MontgomeryForm.h>
#include <hurchalla/modular_arithmetic/modular_pow.h>

#include <gtest/gtest.h>
#include <ios>
#include <format>

using primefield::mod_pow;
using primefield::mod_pow_lim;
using hurchalla::MontgomeryForm;


TEST(ModularExponentiation, BySquaring_FullRange)
{
  const uint64_t p1 = 999999937ull;
  const uint64_t p2 = 1000000007ull;
  const uint64_t p3 = 1000000009ull;

  uint64_t pow_3_17 = 1ull;
  for (int i=0; i < 17; i++)
  {
    pow_3_17 *= 3ull;
  }

  EXPECT_EQ(pow_3_17, mod_pow<uint64_t>(3, 17, p1));
  EXPECT_EQ(1ULL<<18ULL, mod_pow<uint64_t>(2, 18, p1));
  EXPECT_EQ(1ULL<<19ULL, mod_pow<uint64_t>(2, 19, p1));

  // Verify Fermat's little
  for (uint64_t b = 1; b < 100000; b++)
  {
      EXPECT_EQ(b, mod_pow<uint64_t>(b, p1, p1));
      EXPECT_EQ(b, mod_pow<uint64_t>(b, p2, p2));
      EXPECT_EQ(b, mod_pow<uint64_t>(b, p3, p3));
  }
}

TEST(ModularExponentiation, BySquaring_LimitedRange)
{
  const uint64_t p1 = 999999937ull;
  const uint64_t p2 = 1000000007ull;
  const uint64_t p3 = 1000000009ull;

  uint64_t pow_3_17 = 1ull;
  for (int i=0; i < 17; i++)
  {
    pow_3_17 *= 3ull;
  }

  EXPECT_EQ(pow_3_17, mod_pow_lim<uint64_t>(3, 17, p1));
  EXPECT_EQ(1ULL<<18ULL, mod_pow_lim<uint64_t>(2, 18, p1));
  EXPECT_EQ(1ULL<<19ULL, mod_pow_lim<uint64_t>(2, 19, p1));

  // Verify Fermat's little
  for (uint64_t b = 1; b < 10000000; b++)
  {
      EXPECT_EQ(b, mod_pow_lim<uint64_t>(b, p1, p1));
      EXPECT_EQ(b, mod_pow_lim<uint64_t>(b, p2, p2));
      EXPECT_EQ(b, mod_pow_lim<uint64_t>(b, p3, p3));
  }
}

TEST(ModularExponentiation, ClockworkModular)
{
  const uint64_t p1 = 999999937ull;
  const uint64_t p2 = 1000000007ull;
  const uint64_t p3 = 1000000009ull;

  uint64_t pow_3_17 = 1ull;
  for (int i=0; i < 17; i++)
  {
    pow_3_17 *= 3ull;
  }

  EXPECT_EQ(pow_3_17, hurchalla::modular_pow<uint64_t>(3, 17, p1));
  EXPECT_EQ(1ULL<<18ULL, hurchalla::modular_pow<uint64_t>(2, 18, p1));
  EXPECT_EQ(1ULL<<19ULL, hurchalla::modular_pow<uint64_t>(2, 19, p1));

  // Verify Fermat's little
  for (uint64_t b = 1; b < 10000000; b++)
  {
      EXPECT_EQ(b, hurchalla::modular_pow<uint64_t>(b, p1, p1));
      EXPECT_EQ(b, hurchalla::modular_pow<uint64_t>(b, p2, p2));
      EXPECT_EQ(b, hurchalla::modular_pow<uint64_t>(b, p3, p3));
  }
}

TEST(ModularExponentiation, ClockworkMontgomery)
{
  const uint64_t p1 = 999999937ull;
  const uint64_t p2 = 1000000007ull;
  const uint64_t p3 = 1000000009ull;

  auto mf1 = MontgomeryForm(p1);
  auto mf2 = MontgomeryForm(p2);
  auto mf3 = MontgomeryForm(p3);

  // Verify Fermat's little
  for (uint64_t b = 1; b < 10000000; b++)
  {
      EXPECT_EQ(mf1.getUnityValue(), mf1.getCanonicalValue(mf1.pow(mf1.convertIn(b), p1 - 1)));
      EXPECT_EQ(mf2.getUnityValue(), mf2.getCanonicalValue(mf2.pow(mf2.convertIn(b), p2 - 1)));
      EXPECT_EQ(mf3.getUnityValue(), mf3.getCanonicalValue(mf3.pow(mf3.convertIn(b), p3 - 1)));
  }
}
