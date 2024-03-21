// Just a simple check that the ClockWork modular arithmetics library
// seems to work.

#include <hurchalla/montgomery_arithmetic/MontgomeryForm.h>
#include <hurchalla/modular_arithmetic/modular_multiplication.h>

#include <gtest/gtest.h>

using hurchalla::MontgomeryForm;


uint64_t cw_mul(uint64_t a, uint64_t b, uint64_t m)
{
  return hurchalla::modular_multiplication_prereduced_inputs<uint64_t>(a,b,m);
}


TEST(Clockwork, Basic) {

  // Loop over some modulos checking basic arithmetic in montgomery
  uint64_t modulus = 3;
  for (int i = 0; i < 1000; i++)
  {
    // Note: Clockwork modulos need to be < 2**63
    modulus = (17 * modulus) % (1ULL << 63ULL);
    uint64_t a = 3;
    uint64_t b = 11;
    for (int j = 0; j < 1000; j++)
    {
      MontgomeryForm<uint64_t> mf(modulus);
      auto m_a = mf.convertIn(a);
      auto m_b = mf.convertIn(b);
      // Addition
      EXPECT_EQ(mf.convertOut(mf.add(m_a, m_b)), (a+b) % modulus) <<
      "Failure adding " << a << " and " << b;

      // Multiplication
      auto m_prod = mf.multiply(m_a, m_b);
      EXPECT_EQ(mf.convertOut(m_prod), cw_mul(a, b, modulus));

      a = (19*a) % modulus;
      b = (57*b) % modulus;
    }
  }
}

TEST(Clockwork, LargeModulo)
{
  // const uint64_t p = 18446744073709551557;
}
