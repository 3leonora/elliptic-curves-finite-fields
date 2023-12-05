#include <primefield/traits.hpp>
#include <primefield/inverse.hpp>

#include <gtest/gtest.h>
#include <ios>

using primefield::uinvert;

TEST(BasicUInvert, Inverse)
{
  EXPECT_EQ(uinvert(14, 31), 20);
  EXPECT_EQ(uinvert(2U, 4294967291U), 2147483646U);
  EXPECT_EQ(uinvert(2UL, 18361375334787046697UL), 9180687667393523349UL);
  EXPECT_THROW(uinvert(6, 21), primefield::NoInverseError);
}
