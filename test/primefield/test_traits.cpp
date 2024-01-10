#include <gtest/gtest.h>
#include <primefield/traits.hpp>

#include <type_traits>
#include <cinttypes>

using primefield::make_next_rank_t;
using primefield::make_next_signed_rank_t;

// Verify that the promotion to next higher ranked signed
// type works. This can be done already at compile time!
static_assert(
  std::is_signed_v<make_next_signed_rank_t<uint8_t> > == true
  and std::is_signed_v<make_next_signed_rank_t<uint16_t> > == true
  and std::is_signed_v<make_next_signed_rank_t<uint32_t> > == true
  // below doesn't work in either clang or g++
  // and std::is_signed_v<make_next_signed_rank_t<uint64_t> > == true
);


TEST(Traits, IncRank)
{
  using t16 = uint16_t;
  // Test that sizes of an increased rank is as expected!
  EXPECT_EQ(sizeof(make_next_rank_t<uint8_t>), 2);
  EXPECT_EQ(sizeof(make_next_rank_t<uint16_t>), 4);
  EXPECT_EQ(sizeof(make_next_rank_t<t16>), 4);
  EXPECT_EQ(sizeof(make_next_rank_t<uint32_t>), 8);
  EXPECT_EQ(sizeof(make_next_rank_t<uint64_t>), 16);
  EXPECT_EQ(sizeof(make_next_signed_rank_t<uint8_t>), 2);
  EXPECT_EQ(sizeof(make_next_signed_rank_t<uint16_t>), 4);
  EXPECT_EQ(sizeof(make_next_signed_rank_t<uint32_t>), 8);
  EXPECT_EQ(sizeof(make_next_signed_rank_t<uint64_t>), 16);
}
